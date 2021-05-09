#include "NPC.h"
#include "PriorityQueue.h"

NPC::NPC() {
	goals = std::list<Goal>();
	for (auto e : expanded) {
		e = std::vector<TileNode>();
	}
	type = 1;
}

NPC::TileNode NPC::getCharacterCell() const {
	TileNode n;
	n.cell[0] = round(me->getPosition().x / tm->getTileSizeWorld().x);
	n.cell[1] = round(me->getPosition().y / tm->getTileSizeWorld().y) + 1;
	n.data.tile = Tiles::AIR;
	n.heuristic = 0;
	n.cost = 0;
	return n;
}

void NPC::setTileMap(TileMapPtr tm) {
	this->tm = tm;
}

void NPC::addGoal(const sf::Vector2f & goalPos, const float goalRadius) {
	Goal g;
	g.position = goalPos;
	g.radius = goalRadius;
	int length;
	goalMtx.lock();
	bool mustPush = false;
	if (goals.empty()) {
		goals.push_back(g);
	}
	else {
		mustPush = currentGoal[0] == nullptr || *currentGoal[0] != g;
		mustPush &= currentGoal[1] == nullptr || *currentGoal[1] != g;
		//Iterate over last two elements
		auto it = goals.rbegin();
		for (int i = 0; i < 2; i++) {
			if (i >= goals.size()) break;
			mustPush &= *it != g;
			++it;
		}
		if (mustPush) {
			goals.push_back(g);
		}
	}
	goalMtx.unlock();
}

int NPC::findExpanded(const TileNode & n, const int n_path) const {
	int i = 0;
	for (auto node : expanded[n_path]) {
		if (equal(node, n)) break;
		i++;
	}
	return i;
}

float NPC::heuristic(const TileNode& n, const Goal& goal) const
{
	//Shortest distance to circle
	float a = n.cell[0] * tm->getTileSizeWorld().x - goal.position.x;
	float b = n.cell[1] * tm->getTileSizeWorld().y - goal.position.y;
	return abs(sqrt(a*a + b*b) - goal.radius);
}

bool NPC::inBounds(const int x, const int j) const {
	return x >= 0 && j >= 0 && x < tm->getWidth() && j < tm->getHeight();
}

int NPC::updateExpanded(const TileNode& current, TileNode n, const int n_path) {
	int i = findExpanded(n, n_path);
	if (i < expanded[n_path].size() && n.cost < expanded[n_path][i].cost) {
		expanded[n_path][i].cost = n.cost;
		expanded[n_path][i].heuristic = n.heuristic;
		expanded[n_path][i].data = n.data;
		expanded[n_path][i].prev = std::make_shared<TileNode>(current);
	}
	return i;
}

float NPC::cost(const TileNode & current, const TileNode & next) const {
	if (!inBounds(next.cell[0], next.cell[1])
		|| next.data.tile != Tiles::AIR) {
		return INFINITY;
	}
	else if (current.cell[1] > next.cell[1]) {
		return FREE_FALL_COST;
	}
	return RUN_COST;
}

void NPC::calculateJumpNeighbours(const TileNode & current, const Goal& goal, const int n_path) {
	TileNode n;
	float dist, baseCost;
	bool jumps;
	bool onRightSide = false, expandVertical = true;
	sf::Vector2i lowLimits = { 0,0 }, highLimits = { 0,0 };
	int leftObstacle, rightObstacle;
	Tiles::Collidable underMe = Tiles::FLOOR;
	if (inBounds(current.cell[0], current.cell[1] + 1)) {
		underMe = tm->getTile(current.cell[0], current.cell[1] + 1);
	}
	 
	if (underMe == Tiles::FLOOR) {
		lowLimits.x = -glb::jump1Tiles.x;
		highLimits.x = glb::jump1Tiles.x;
		lowLimits.y = 1;
		highLimits.y = glb::jump1Tiles.y;
		baseCost = JUMP_COST_BASE_1;
		jumps = 1;
	}
	else if (underMe == Tiles::AIR) {
		lowLimits.x = -glb::jump2Tiles.x;
		highLimits.x = glb::jump2Tiles.x;
		lowLimits.y = 1;
		highLimits.y = glb::jump2Tiles.y;
		baseCost = JUMP_COST_BASE_2;
		jumps = 0;
	}
	
	int x = 0;
	int y = lowLimits.y;
	int forbiddenX = 0, leftForbiddenY = 0, rightForbiddenY = 0;

	//Loop first from bottom to top, doing first the left side (moving to the left) and then
	//the right side (moving to the right)
	while (y < highLimits.y) {
		dist = float(x*x) / float(highLimits.x * highLimits.x) + float(y*y) / float(highLimits.y * highLimits.y);
		n.cell[0] = current.cell[0] + x;
		//NOTE: y is used in the NATURAL WAY, so we have to substract here
		n.cell[1] = current.cell[1] - y;

		//We must be in bounds
		if (inBounds(n.cell[0], n.cell[1])) {
			n.data.tile = tm->getTile(n.cell[0], n.cell[1]);

			//We must be inside the ellipse
			if (dist <= 1
				//In the case we were jumping in a vertical direction, expandVertical must be true
				&& (x != 0 || expandVertical)
				//If we are constrained (leftForbiddenY > 0) and we are in the left side, the position can't be in the range
				// [leftObstacle - forbiddenX, leftObstacle]
				&& (leftForbiddenY <= 0 || (onRightSide || !utils::inRange(current.cell[0] + x, leftObstacle - forbiddenX, leftObstacle)))
				//If we are constrained (rightForbiddenY > 0) and we are in the right side, the position can't be in the range
				// [rightObstacle, rightObstacle + forbiddenX]
				&& (rightForbiddenY <= 0 || (!onRightSide || !utils::inRange(current.cell[0] + x, rightObstacle, rightObstacle + forbiddenX)))) {

				//This tile and the tile over this one must be empty (otherwise we can't go there)
				if (n.data.tile == Tiles::AIR && (inBounds(n.cell[0], n.cell[1] - 1) && tm->getTile(n.cell[0], n.cell[1] - 1) == Tiles::AIR)) {
					n.cost = current.cost + dist * JUMP_COST_PER_DISTANCE_UNIT + baseCost;
					n.heuristic = heuristic(n, goal);
					n.prev = std::make_shared<TileNode>(current);
					n.data.jumps = jumps;
					if (detectDirectionChange(n, current)) {
						n.cost += DIRECTION_CHANGE_COST;
					}
					if (updateExpanded(current, n, n_path) >= expanded[n_path].size() && n.cost != INFINITY) {
						frontier[n_path].insert(n);
					}
				}
				else {
					if (x == 0) {
						expandVertical = false;
					}
					if (!onRightSide) {
						//The length of the obstacle's forbidden area will be proportional to its height
						forbiddenX = highLimits.y - n.cell[1];
						//It will have a height of 2
						leftForbiddenY = 2;
						leftObstacle = n.cell[0];
					}
					else {
						//The length of the obstacle's forbidden area will be proportional to its height
						forbiddenX = highLimits.y - n.cell[1];
						//It will have a height of 2
						rightForbiddenY = 2;
						rightObstacle = n.cell[0];
					}
				}
			}
		}
		if (!onRightSide) {
			//We're on the left side, expand to the left
			x--;
			if (x < lowLimits.x) {
				onRightSide = true;
				x = 1;
			}

		}
		else {
			//We're on the right side, expand to the right
			x++;
			if (x > highLimits.x) {
				onRightSide = false;
				x = 0;
				y++;
				if (leftForbiddenY > 0) leftForbiddenY--;
				if (rightForbiddenY > 0) rightForbiddenY--;
			}
		}
	}
}

void NPC::calculateHookNeighbours(const bool right, const TileNode& current, const Goal& goal, const int n_path) {
	TileNode aux, hook;
	int i, radius = 1;
	Tiles::Collidable type = Tiles::AIR;
	bool stop = false;

	while (!stop) {
		if (right) {
			//Search tiles in a 45º angle
			hook.cell[0] = current.cell[0] + radius;
		}
		else {
			//Search tiles in a 135º angle
			hook.cell[0] = current.cell[0] - radius;
		}
		hook.cell[1] = current.cell[1] - radius;
		//Check the upper cell for non-air cells
		if (inBounds(hook.cell[0], hook.cell[1] - 1)) {
			type = tm->getTile(hook.cell[0], hook.cell[1] - 1);
			stop = (type != Tiles::AIR);
		}
		else {
			stop = true;
		}
		radius++;
	}

	float cost = HOOK_COST;
	float angle;
	float radius2 = radius * radius;
	stop = false;
	//If we found a grapplable surface
	if (type == Tiles::GRAPPLABLE) {
		//Loop through all of the tiles on the edge of a circle of radius = radius
		//Max horizontal position is for an angle of -45 (or -135) degrees
		for (int x = -radius * glb::cosine45; x <= radius && !stop; ++x) {
			// Max height is for an angle of 30 (or 150) degrees, whose sine = 0.5
			for (int y = -radius; y <= radius * 0.5f && !stop; ++y) {
				if (abs(x * x + y * y - radius2) > THRESHOLD_PER_RADIUS_UNIT * radius) continue;
				//Do it ONLY for the positive solution of y 
				angle = utils::degrees(atan2(y, x));
				if (angle > 30 || angle < -135) continue;
				//For a hook facing right, the angle will be in between 30 and -135
				if (right) {
					aux.cell[0] = hook.cell[0] + x;
				}
				//For a hook facing left, the angle will be in between -45 and -180 or in between 150 and 180 degrees
				else if (!right) {
					aux.cell[0] = hook.cell[0] - x;
				}
				else {
					//Any of those conditions must be true for it to be in the ranges
					continue;
				}
				
				aux.cell[1] = hook.cell[1] - y;
				if (inBounds(aux.cell[0], aux.cell[1])) {
					aux.data.tile = tm->getTile(aux.cell[0], aux.cell[1]);
				}
				//If this one, and the one on top are air, we can use the hook here
				if (aux.data.tile == Tiles::AIR
					&& inBounds(aux.cell[0], aux.cell[1] -1) && tm->getTile(aux.cell[0], aux.cell[1] -1 == Tiles::AIR)) {
					aux.cost = current.cost + cost;
					aux.heuristic = heuristic(aux, goal);
					aux.data.isHooking = true;
					aux.prev = std::make_shared<TileNode>(current);
					if (detectDirectionChange(aux, current)) {
						aux.cost += DIRECTION_CHANGE_COST;
					}
					//Add to frontier if it hasn't been expanded yet, or if the cost is smaller than the previous
					if (updateExpanded(current, aux, n_path) >= expanded[n_path].size() && aux.cost != INFINITY) {
						frontier[n_path].insert(aux);
						cost += HOOK_COST;
					}
				}
				else {
					stop = true;
				}
			}
		}
	}
}

void NPC::calculateWallJumpNeighbours(const bool right, TileNode& current, const Goal& goal, const int n_path) {
	TileNode n, aux;
	bool canWallJump = false, stop = false;
	//Check the opposite side for wall jump walls
	if ((right && current.data.canWallJumpRight < 0) || (!right && current.data.canWallJumpLeft < 0)) {
		if (right) {
			aux.cell[0] = current.cell[0] - 1;
		}
		else {
			aux.cell[0] = current.cell[0] + 1;
		}

		for (int y = -1; y <= 1 && !canWallJump; y++) {
			aux.cell[1] = current.cell[1] + y;
			if (inBounds(aux.cell[0], aux.cell[1])) {
				aux.data.tile = tm->getTile(aux.cell[0], aux.cell[1]);
				canWallJump |= (right && aux.data.tile == Tiles::JUMP_WALL_R) || (!right && aux.data.tile == Tiles::JUMP_WALL_L);
			}
		}
	}

	if (canWallJump || (right && current.data.canWallJumpRight == 1) || (!right && current.data.canWallJumpLeft == 1)) {
		//Make sure it is equal to 1
		if (right) {
			current.data.canWallJumpRight = 1;
		}
		else {
			current.data.canWallJumpLeft = 1;
		}
		
		for (int x = 2; x <= 4 && !stop; x++) {
			for (int y = 0; y <= 1 && !stop; y++) {

				if (right) {
					n.cell[0] = current.cell[0] + x;
				}
				else{
					n.cell[0] = current.cell[0] - x;
				}
				
				n.cell[1] = current.cell[1] - y;
				if (inBounds(n.cell[0], n.cell[1])) {
					n.data.tile = tm->getTile(n.cell[0], n.cell[1]);
					if (n.data.tile == Tiles::AIR) {
						n.data.jumps = 1;
						n.data.isHooking = false;
						n.cost = current.cost + WALL_JUMP_COST + abs(x) * JUMP_COST_PER_DISTANCE_UNIT;
						n.heuristic = heuristic(n, goal);
						n.prev = std::make_shared<TileNode>(current);
						if (updateExpanded(current, n, n_path) >= expanded[n_path].size()) {
							frontier[n_path].insert(n);
						}
					}
					else {
						stop = true;
					}
				}
			}
		}
	}
	else {
		if (right) {
			current.data.canWallJumpRight = 0;
		}
		else {
			current.data.canWallJumpLeft = 0;
		}
		
	}
}

std::list<std::shared_ptr<NPC::TileNode> > NPC::buildPath (TileNode foundGoal) {
	std::list<std::shared_ptr<TileNode> > newPath = std::list<std::shared_ptr<TileNode> >();
	newPath.push_front(std::make_shared<TileNode>(foundGoal));
	std::shared_ptr<TileNode> next = foundGoal.prev;
	while (next != nullptr) {
		std::cout << next->cell[0] << " " << next->cell[1] << " " << next->cost << " " << next->heuristic << std::endl;
		newPath.push_front(next);
		next = next->prev;
	}
	return newPath;
}

bool NPC::isGoal(const TileNode & current, const Goal& goal) const {
	float a = current.cell[0] * tm->getTileSizeWorld().x - goal.position.x;
	float b = current.cell[1] * tm->getTileSizeWorld().y - goal.position.y;
	return abs(sqrt(a*a + b * b)) < goal.radius;
}

void NPC::updateGoals() {
	goalMtx.lock();
	if (currentGoal[0] == nullptr) {
		if (currentGoal[1] != nullptr) {
			currentGoal[0] = currentGoal[1];
			currentGoal[1] = nullptr;
		}
		else if (!goals.empty()) {
			currentGoal[0] = std::make_shared<Goal>(goals.front());
			goals.pop_front();
		}
	}
	if (currentGoal[1] == nullptr && !goals.empty()) {
		currentGoal[1] = std::make_shared<Goal>(goals.front());
		goals.pop_front();
	}
	goalMtx.unlock();
}

bool NPC::nodeWasReached(const TileNode & n, const float closenessThreshold) const {
	return ((me->canWallJump() || (n.data.canWallJumpLeft != 1 && n.data.canWallJumpRight != 1)) && distance(getCharacterCell(), n) <= closenessThreshold);
}

bool NPC::detectDirectionChange(const TileNode & n, const TileNode & current)
{
	int diff1, diff0;
	if (current.prev != nullptr) {
		diff1 = n.cell[0] - current.cell[0];
		diff0 = current.cell[0] - current.prev->cell[0];
	}
	else {
		//Can only check if we have a previous node
		return false;
	}
	 
	//There has been horizontal movement in both steps
	//The difference in the horizontal coordinates between n and current, and current and its previous, have opposite signs
	//The direction change wasn't produced because of a wall jump
	return ((diff1 >= 0) ^ (diff0 >= 0)) && (current.data.canWallJumpRight != 1 || current.data.canWallJumpLeft != 1);
}

float NPC::expandToNeighbour(const TileNode& current, const Goal& goal, const int dx, const int dy, const int n_path) {
	TileNode next;

	next.cell[0] = dx + current.cell[0];
	next.cell[1] = dy + current.cell[1];
	next.data.jumps = current.data.jumps;

	if (inBounds(next.cell[0], next.cell[1])) {
		next.data.tile = tm->getTile(next.cell[0], next.cell[1]);
	
		if (inBounds(next.cell[0], next.cell[1]) && tm->getTile(next.cell[0], next.cell[1] - 1) != Tiles::AIR) {
			next.cost = current.cost + SLIDE_COST;
			next.data.isSliding = true;
		}
		next.cost = current.cost + cost(current, next);
		next.prev = std::make_shared<TileNode>(current);
		if (detectDirectionChange(next, current)) {
			next.cost += DIRECTION_CHANGE_COST;
		}
		next.heuristic = heuristic(next, goal);
		if (updateExpanded(current, next, n_path) >= expanded[n_path].size() && next.cost != INFINITY) {
			frontier[n_path].insert(next);
		}
		return next.cost;
	}
	else {
		return INFINITY;
	}
	
}

void NPC::plan() {
	TileNode current, nextToMe, goal0;
	std::unique_ptr<TileNode> prev = nullptr;
	Tiles::Collidable underMe;
	float auxCost;
	int n_path = 0;

	//Have to do "replan path"
	if (pathFound[0] == 1) {
		if (pathFound[1] == 1) return;
		else {
			n_path = 1;
		}
	}

	//Check nobody else is planning this path
	choiceMtx.lock();
	if (!planningPath[n_path]) {
		planningPath[n_path] = true;
		updateGoals();
		choiceMtx.unlock();
	}
	//Choose the next one in case someone is already planning it
	else if(!planningPath[(n_path + 1) % 2] && !pathFound[(n_path + 1) % 2]){
		n_path = (n_path + 1) % 2;
		planningPath[n_path] = true;
		updateGoals();
		choiceMtx.unlock();
	}
	else {
		choiceMtx.unlock();
		return;
	}


	if (pathFound[0] == -1) {
		std::cout << "Replanning..." << std::endl;
	}

	frontier[n_path].clear();
	expanded[n_path].clear();

	std::shared_ptr<Goal> goal, prevGoal;
	TileNode goalNode;
	goalMtx.lock();
	if (currentGoal[n_path] == nullptr) {
		goalMtx.unlock();
		choiceMtx.lock();
		planningPath[n_path] = false;
		choiceMtx.unlock();
		return;
	}
	goal = currentGoal[n_path];

	//Start from current position, if it's the first path.
	//Start from previous goal, if it's the second one.
	if (n_path == 0) {
		goalMtx.unlock();
		frontier[n_path].safePush(getCharacterCell());
	}
	else if (currentGoal[0] == nullptr) {
		goalMtx.unlock();
		choiceMtx.lock();
		planningPath[n_path] = false;
		choiceMtx.unlock();
		return;
	}
	else {
		goal0.cell[0] = currentGoal[0]->position.x;
		goal0.cell[1] = currentGoal[0]->position.y;
		goalMtx.unlock();
		frontier[n_path].safePush(goal0);
	}

	goalNode.cell[0] = int(goal->position.x) / tm->getTileSizeWorld().x;
	goalNode.cell[1] = int(goal->position.y) / tm->getTileSizeWorld().y;

	goalNode.data.tile = Tiles::AIR;

	while (!frontier[n_path].safeEmpty()) {
		current = frontier[n_path].popReturn();
		expanded[n_path].push_back(current);
		auxCost = INFINITY;

		//Don't let expand to hook nodes between different hooks
		if (prev != nullptr && prev->prev != current.prev && prev->data.isHooking && current.data.isHooking) continue;

		if (isGoal(current, *goal)) {
			stopFollowing = true;	//Tell moving thread to stop following the path
			std::list<std::shared_ptr<TileNode> > newPath;
			newPath = buildPath(current);
			pathMtx[n_path].lock();
			path[n_path] = newPath;	//Once it's safe, replace the old path with the new one
			pathMtx[n_path].unlock();
			break;
		}

		//Expand to the wall jump neighbours if we can do it
		calculateWallJumpNeighbours(true, current, *goal, n_path);
		calculateWallJumpNeighbours(false, current, *goal, n_path);

		//Expand to the jumping neighbours only if we can jump and we can't wall jump
		if (current.data.canJump() && current.data.canWallJumpLeft != 1 && current.data.canWallJumpRight != 1) {
			calculateJumpNeighbours(current, *goal, n_path);
		}
		//Expand to the hook neighbours only if we aren't using the hook
		if (!current.data.isHooking) {
			calculateHookNeighbours(true, current, *goal, n_path);
			calculateHookNeighbours(false, current, *goal, n_path);
		}

		if (inBounds(current.cell[0], current.cell[1] + 1)) {
			underMe = tm->getTile(current.cell[0], current.cell[1] + 1);
		}
		else underMe = Tiles::FLOOR;

		if (underMe == Tiles::FLOOR) {
			current.data.jumps = 2;
		}

		for (int x = -1; x <= 1; x++) {
			if (underMe == Tiles::RAMP_UP || underMe == Tiles::STAIRS_UP) {
				expandToNeighbour(current, *goal, x, -x, n_path);
			}
			else if (underMe == Tiles::RAMP_DOWN || underMe == Tiles::STAIRS_DOWN) {
				expandToNeighbour(current, *goal, x, x, n_path);
			}
			else if (underMe == Tiles::AIR) {
				nextToMe.cell[0] = current.cell[0] + x;
				nextToMe.cell[1] = current.cell[1];
				if (inBounds(nextToMe.cell[0], nextToMe.cell[1]) && tm->getTile(nextToMe.cell[0], nextToMe.cell[1]) == Tiles::AIR) {
					auxCost = expandToNeighbour(current, *goal, x, 1, n_path);
					if (auxCost != INFINITY) {
						expandToNeighbour(current, *goal, 2 * x, 1, n_path);
					}
				}
			}
			else if (x != 0) {
				expandToNeighbour(current, *goal, x, 0, n_path);
			}
			
		}
		prev = std::make_unique<TileNode>(current);
	}
	if (!path[n_path].empty()) {
		pathFound[n_path] = 1;
	}
	choiceMtx.lock();
	planningPath[n_path] = false;
	choiceMtx.unlock();

	goalMtx.lock();
	currentGoal[n_path] = nullptr;
	goalMtx.unlock();
}


void NPC::doBasicMovement(const TileNode& current, const TileNode& n, bool& jumped, bool block) {
	bool right = getCharacterCell().cell[0] < n.cell[0];
	do {
		if (current.data.isSliding || n.data.isSliding) {
			me->slide();
		}
		else {
			if (current.data.jumps > n.data.jumps || getCharacterCell().cell[1] - n.cell[1] > 2) {
				//Jump (or double jump)
				if (!jumped) {
					me->startJumping();
					jumped = true;
				}
			}
			else if(getCharacterCell().cell[1] - n.cell[1] < 0){
				me->stopJumping();
			}
			if ((right && getCharacterCell().cell[0] < n.cell[0]) || (!right && getCharacterCell().cell[0] > n.cell[0])) {
				me->run(right);
			}
			else {
				me->stop();
			}
		}
	} while (block && !stopFollowing && distance(getCharacterCell(), n) <= FARNESS_THRESHOLD && !nodeWasReached(n, CLOSENESS_THRESHOLD));
}

void NPC::followPath() {
	TileNode current = getCharacterCell();
	Tiles::Collidable underMe;
	int step = 0;
	bool jumped = false;
	stopFollowing = false;
	float dist = 0;

	sf::Time sleeptime = sf::seconds(0.05);
	pathMtx[0].lock();
	if (pathFound[0] == 1) {
		for (auto& next : path[0]) {
			if (stopFollowing) {
				break;	//A better path was found
			}
			jumped = false;
			if (current.data.canWallJumpLeft == 1 || current.data.canWallJumpRight == 1) {
				while (!stopFollowing && dist <= FARNESS_THRESHOLD && !nodeWasReached(*next, 1)) {
					//Wall jump
					if (me->canWallJump()) {
						me->startJumping();
					}
					doBasicMovement(current, *next, jumped, false);
					dist = distance(getCharacterCell(), *next);
				}
			}
			else if (!current.data.isHooking && next->data.isHooking) {
				//Use hook
				me->useHook(true);
				while (!stopFollowing && dist && !nodeWasReached(*next, CLOSENESS_THRESHOLD)) {
					sf::sleep(sleeptime);
					dist = distance(getCharacterCell(), *next);
				}
			}
			else if (current.data.isHooking && !next->data.isHooking) {
				//Stop using hook
				me->useHook(false);
				while (!stopFollowing && dist <= FARNESS_THRESHOLD && !nodeWasReached(*next, CLOSENESS_THRESHOLD)) {
					sf::sleep(sleeptime);
					dist = distance(getCharacterCell(), *next);
				}
			}
			else {
				doBasicMovement(current, *next, jumped, true);
				dist = distance(getCharacterCell(), *next);
			}
			step++;
			current = *next;
			std::cout << step << std::endl;
		}
		if (dist > FARNESS_THRESHOLD) {
			pathFound[0] = -1; //Reset so we can replan
		}
		else {
			
			if (pathFound[1] == 1) {
				pathFound[0] = 1;	//Next part of the path was already planned
				//Stitch the two paths together and save path[1] to path[0]
				auto last = path[0].back();
				auto first = path[1].front();
				first = last;
				std::copy(std::begin(path[1]), std::end(path[1]), std::begin(path[0]));
				//Make sure we start planning the next part
				path[1].clear();
				pathFound[1] = 0;
			}
			else {
				pathFound[0] = 0;	//We reached the end, so we make sure we replan
			}
		}
	}
	pathMtx[0].unlock();
}

int NPC::getPathFound(int i) const {
	return pathFound[i];
}

std::list<selbaward::Line> NPC::debugLines() {
	std::list<selbaward::Line> lines = std::list<selbaward::Line>();
	sf::Vector2u size = tm->getTileSizeWorld();
	for (auto n : path[0]) {
		if (n->prev == NULL) continue;
		selbaward::Line l;
		sf::Vector2f start = {(float)n->prev->cell[0] * size.x, (float)n->prev->cell[1] * size.y };
		sf::Vector2f end = { (float)n->cell[0] * size.x, (float)n->cell[1] * size.y };
		l.setPoints(start, end);
		if (n->data.isHooking) {
			l.setColor(sf::Color::Blue);
		}
		else if(!n->data.canJump()){
			l.setColor(sf::Color::Red);
		}
		else {
			l.setColor(sf::Color::Green);
		}
		
		l.setThickness(2.0f);
		lines.push_back(l);
	}
	return lines;
}

std::list<sf::RectangleShape> NPC::debugExpanded() {
	sf::Vector2u size = tm->getTileSizeWorld();
	std::list<sf::RectangleShape> squares;
	if (expanded[0].empty()) return squares;
	auto last = expanded[0].end() - 1;
	int max = last->cost + last->heuristic;
	for (auto n : expanded[0]) {
		if (n.prev == NULL) continue;
		sf::RectangleShape r;
		
		r.setSize(sf::Vector2f(size.x, size.y));
		sf::Vector2f end = { (float)n.cell[0] * size.x, (float)n.cell[1] * size.y };
		r.setPosition(end);
		if (n.data.isHooking) {
			r.setFillColor(sf::Color(0, 0, 255, 255 / 2));
		}
		else if (!n.data.canJump()) {
			r.setFillColor(sf::Color(255, 0, 0, 255 / 2));
		}
		else {
			r.setFillColor(sf::Color(0, 255, 0, 255 / 2));
		}
		squares.push_back(r);
	}
	return squares;
}

std::list<sf::RectangleShape> NPC::debugHook() {
	int radius = 15;
	std::list<sf::RectangleShape> squares;
	sf::Vector2u size = tm->getTileSizeWorld();
	TileNode current = getCharacterCell();
	calculateHookNeighbours(true, current, *currentGoal[0], 0);
	TileNode f;

	while(!frontier[0].safeEmpty()) {
		f = frontier[0].popReturn();
		sf::RectangleShape r;
		r.setSize(sf::Vector2f(size.x, size.y));
		r.setPosition(sf::Vector2f(f.cell[0] *size.x, f.cell[1]*size.y));
		r.setFillColor(sf::Color::Magenta);
		squares.push_back(r);
		
	}
	return squares;
}
