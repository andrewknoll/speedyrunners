#include "NPC.h"
#include "PriorityQueue.h"

NPC::NPC() {
	frontier = TilePriorityQueue();
	expanded = std::vector<TileNode>();
	type = 1;
}

NPC::TileNode NPC::getCharacterCell() const {
	TileNode n;
	n.cell[0] = int(me->getPosition().x) / tm->getTileSizeWorld().x;
	n.cell[1] = int(me->getPosition().y) / tm->getTileSizeWorld().y;
	n.data.tile = Tiles::AIR;
	n.heuristic = 0;
	n.cost = 0;
	return n;
}

void NPC::setTileMap(TileMapPtr tm) {
	this->tm = tm;
}

NPC::TileNode NPC::frontierPop() {
	TileNode n = frontier.top();
	frontier.pop();
	return n;
}

int NPC::findExpanded(const TileNode & n) const {
	int i = 0;
	for (auto node : expanded) {
		if (sameCell(node, n)) break;
		i++;
	}
	return i;
}

float NPC::heuristic(const TileNode& n, sf::Vector2f goalPos, float goalRadius) const
{
	//Shortest distance to circle
	float a = n.cell[0] * tm->getTileSizeWorld().x - goalPos.x;
	float b = n.cell[1] * tm->getTileSizeWorld().y - goalPos.y;
	return abs(sqrt(a*a + b*b) - goalRadius);
}

bool NPC::inBounds(const int x, const int j) const {
	return x >= 0 && j >= 0 && x < tm->getWidth() && j < tm->getHeight();
}

int NPC::updateExpanded(const TileNode& current, TileNode n, float heuristic) {
	int i = findExpanded(n);
	if (i < expanded.size() && n.cost < expanded[i].cost) {
		expanded[i].cost = n.cost;
		expanded[i].heuristic = heuristic;
		expanded[i].prev = std::make_shared<TileNode>(current);
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
	else if (inBounds(next.cell[0], next.cell[1]) && tm->getTile(next.cell[0], next.cell[1] - 1) != Tiles::Collidable::AIR) {
		return SLIDE_COST;
	}
	return RUN_COST;
}

void NPC::calculateJumpNeighbours(const TileNode & current, const sf::Vector2f& goalPos, const float goalRadius) {
	TileNode n;
	float dist, baseCost;
	bool onRightSide = false, expandVertical = true;
	sf::Vector2i lowLimits = { 0,0 }, highLimits = { 0,0 };
	int leftObstacle, rightObstacle;
	Tiles::Collidable underMe = tm->getTile(current.cell[0], current.cell[0] + 1);
	if (underMe == Tiles::FLOOR) {
		lowLimits.x = -glb::jump1Tiles.x;
		highLimits.x = glb::jump1Tiles.x;
		lowLimits.y = 1;
		highLimits.y = glb::jump1Tiles.y;
		baseCost = JUMP_COST_BASE_1;
	}
	else if (underMe == Tiles::AIR) {
		lowLimits.x = -glb::jump2Tiles.x;
		highLimits.x = glb::jump2Tiles.x;
		lowLimits.y = 1;
		highLimits.y = glb::jump2Tiles.y;
		baseCost = JUMP_COST_BASE_2;
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
		if (inBounds(n.cell[0], n.cell[1])
			//We must be inside the ellipse
			&& dist <= 1 
			//In the case we were jumping in a vertical direction, expandVertical must be true
			&& (x != 0 || expandVertical) 
			//If we are constrained (leftForbiddenY > 0) and we are in the left side, the position can't be in the range
			// [leftObstacle - forbiddenX, leftObstacle]
			&& (leftForbiddenY <= 0 || (onRightSide || !utils::inRange(current.cell[0] + x, leftObstacle - forbiddenX, leftObstacle)))
			//If we are constrained (rightForbiddenY > 0) and we are in the right side, the position can't be in the range
			// [rightObstacle, rightObstacle + forbiddenX]
			&& (rightForbiddenY <= 0 || (!onRightSide || !utils::inRange(current.cell[0] + x, rightObstacle, rightObstacle + forbiddenX)))) {
			
			n.data.tile = tm->getTile(n.cell[0], n.cell[1]);
			if (n.data.tile == Tiles::AIR) {
				std::cout << dist << " " << x << " " << y << std::endl;
				n.cost = current.cost + dist * JUMP_COST_PER_DISTANCE_UNIT + baseCost;
				n.heuristic = heuristic(n, goalPos, goalRadius);
				n.prev = std::make_shared<TileNode>(current);
				n.data.canJump = false;
				frontier.insert(n);
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

void NPC::calculateHookNeighbours(const TileNode& current, const sf::Vector2f& goalPos, const float goalRadius) {
	TileNode auxR, auxL;
	int i, radius = 0;
	Tiles::Collidable typeR = Tiles::AIR, typeL = Tiles::AIR;
	bool stopR = false, stopL = false;
	do {
		radius++;
		
		if (!stopR) {
			auxR.cell[0] = current.cell[0] + radius;
			auxR.cell[1] = current.cell[1] + radius;

			if (inBounds(auxR.cell[0], auxR.cell[1] - 1)) {
				typeR = tm->getTile(auxR.cell[0], auxR.cell[1] - 1);
				stopR = (typeR == Tiles::AIR);
			}
			else stopR = true;
		}
		if (!stopL) {
			auxL.cell[0] = current.cell[0] - radius;
			auxL.cell[1] = current.cell[1] - radius;
			if (inBounds(auxL.cell[0], auxL.cell[1] - 1)) {
				typeL = tm->getTile(auxL.cell[0], auxL.cell[1] - 1);
				stopL = (typeL == Tiles::AIR);
			}
			else stopR = true;
		}
	} while (!stopR || !stopL);

	float costL = HOOK_COST, costR = HOOK_COST;
	float angle;
	int xL, yL, xR, yR;

	if (typeR == Tiles::GRAPPLABLE || typeL == Tiles::GRAPPLABLE) {
		//Right
		for (int x = -radius; x <= radius; ++x) {
			for (int y = -radius; y <= radius; ++y) {
				if (x*x + y * y > radius * radius) continue;
				angle = utils::degrees(atan2(y, x));
				//Angle must be in between 30 and -180 or equal to 180 degrees
				if (typeR == Tiles::GRAPPLABLE && (angle <= 30 || angle == 180)) {
					xR = x + current.cell[0] + radius;
					yR = x + current.cell[0] + radius;
					if (tm->getTile(xR, yR) == Tiles::AIR) {
						auxR.cost = current.cost + costR;
						auxR.heuristic = heuristic(auxR, goalPos, goalRadius);
						auxR.data.isHooking = true;
						auxR.prev = std::make_shared<TileNode>(current);
						frontier.insert(auxR);
						costR += HOOK_COST;
					}
				}
				if (typeL == Tiles::GRAPPLABLE && (angle <= 0 || angle >= 150)) {
					xL = x + current.cell[0] - radius;
					yL = x + current.cell[0] - radius;
					if (tm->getTile(xR, yR) == Tiles::AIR) {
						auxL.cost = current.cost + costL;
						auxL.heuristic = heuristic(auxR, goalPos, goalRadius);
						auxL.data.isHooking = true;
						auxL.prev = std::make_shared<TileNode>(current);
						frontier.insert(auxL);
						costL += HOOK_COST;
					}
				}
				
			}
		}
	}
}

void NPC::buildPath(TileNode foundGoal) {
	path.push_front(std::make_shared<TileNode>(foundGoal));
	std::shared_ptr<TileNode> next = foundGoal.prev;
	std::cout << foundGoal.cell[0] << " " << foundGoal.cell[1] << next->cost << std::endl;
	while (next != nullptr) {
		std::cout << next->cell[0] << " " << next->cell[1] << " " << next->cost << std::endl;
		path.push_front(next);
		next = next->prev;
	}
}

bool NPC::isGoal(const TileNode & current, const sf::Vector2f& goalPos, const float radius) const {
	float a = current.cell[0] * tm->getTileSizeWorld().x - goalPos.x;
	float b = current.cell[1] * tm->getTileSizeWorld().y - goalPos.y;
	return abs(sqrt(a*a + b * b)) < radius;
}

float NPC::expandToNeighbour(const TileNode& current, const int dx, const int dy, const sf::Vector2f& goalPos, const float goalRadius) {
	TileNode next;
	float h;

	next.cell[0] = dx + current.cell[0];
	next.cell[1] = dy + current.cell[1];
	next.data.canJump = current.data.canJump;

	if (inBounds(next.cell[0], next.cell[1])) {
		next.data.tile = tm->getTile(next.cell[0], next.cell[1]);
	}

	next.cost = current.cost + cost(current, next);
	h = heuristic(next, goalPos, goalRadius);
	if (updateExpanded(current, next, h) >= expanded.size() && next.cost != INFINITY) {
		next.prev = std::make_shared<TileNode>(current);
		next.heuristic = h;
		frontier.insert(next);
	}
	return next.cost;
}

void NPC::play(const sf::Vector2f& goalPos, const float goalRadius) {
	TileNode current;
	Tiles::Collidable underMe, nextToMe;
	float auxCost1, auxCost2;
	int exp;

	frontier.push(getCharacterCell());
	expanded.clear();
	//Have to do "replan path"
	if (!path.empty()) return;

	TileNode goal;
	goal.cell[0] = int(goalPos.x) / tm->getTileSizeWorld().x;
	goal.cell[1] = int(goalPos.y) / tm->getTileSizeWorld().y;
	goal.data.tile = Tiles::AIR;

	while (!frontier.empty()) {
		current = frontierPop();
		expanded.push_back(current);
		auxCost1 = INFINITY;
		auxCost2 = INFINITY;

		if (isGoal(current, goalPos, goalRadius)) {
			frontier.clear();
			buildPath(current);
			break;
		}
		if (current.data.canJump) {
			calculateJumpNeighbours(current, goalPos, goalRadius);
		}

		//calculateHookNeighbours(current, goalPos, goalRadius);

		if (inBounds(current.cell[0], current.cell[1] + 1)) {
			underMe = tm->getTile(current.cell[0], current.cell[1] + 1);
		}
		else underMe = Tiles::FLOOR;

		if (underMe == Tiles::FLOOR) {
			current.data.canJump = true;
		}

		for (int x = -1; x <= 1; x++) {
			if (underMe == Tiles::RAMP_UP || underMe == Tiles::STAIRS_UP) {
				expandToNeighbour(current, x, -x, goalPos, goalRadius);
			}
			else if (underMe == Tiles::RAMP_DOWN || underMe == Tiles::STAIRS_DOWN) {
				expandToNeighbour(current, x, x, goalPos, goalRadius);
			}
			else if (x != 0) {
				auxCost1 = expandToNeighbour(current, x, 0, goalPos, goalRadius);
			}
			if (underMe == Tiles::AIR) {
				if (auxCost1 != INFINITY) {
					auxCost2 = expandToNeighbour(current, x, 1, goalPos, goalRadius);
					if (auxCost2 != INFINITY) {
						expandToNeighbour(current, 2 * x, 1, goalPos, goalRadius);
					}
				}
			}
		}
	}
}

std::list<selbaward::Line> NPC::debugLines() {
	std::list<selbaward::Line> lines = std::list<selbaward::Line>();
	for (auto n : path) {
		if (n->prev == NULL) continue;
		selbaward::Line l;
		sf::Vector2u size = tm->getTileSizeWorld();
		sf::Vector2f start = {(float)n->prev->cell[0] * size.x, (float)n->prev->cell[1] * size.y };
		sf::Vector2f end = { (float)n->cell[0] * size.x, (float)n->cell[1] * size.y };
		l.setPoints(start, end);
		if (n->data.isHooking) {
			l.setColor(sf::Color::Blue);
		}
		else if(!n->data.canJump){
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
