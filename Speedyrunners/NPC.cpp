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
	n.data = Tiles::AIR;
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
		|| next.data != Tiles::AIR) {
		return INFINITY;
	}
	else if (inBounds(next.cell[0], next.cell[1]) && tm->getTile(next.cell[0], next.cell[1] - 1) != Tiles::Collidable::AIR) {
		return SLIDE_COST;
	}
	return RUN_COST;
}

void NPC::calculateHookNeighbours(const TileNode& current) {
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
						frontier.insert(auxR);
						costR += HOOK_COST;
					}
				}
				if (typeL == Tiles::GRAPPLABLE && (angle <= 0 || angle >= 150)) {
					xL = x + current.cell[0] - radius;
					yL = x + current.cell[0] - radius;
					if (tm->getTile(xR, yR) == Tiles::AIR) {
						auxL.cost = current.cost + costL;
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

void NPC::play(const sf::Vector2f& goalPos, const float goalRadius) {
	TileNode current;
	int new_cost;
	int exp;
	float h;

	frontier.push(getCharacterCell());
	expanded.clear();
	//Have to do "replan path"
	if (!path.empty()) return;

	TileNode goal;
	goal.cell[0] = int(goalPos.x) / tm->getTileSizeWorld().x;
	goal.cell[1] = int(goalPos.y) / tm->getTileSizeWorld().y;
	goal.data = Tiles::AIR;

	while (!frontier.empty()) {
		current = frontierPop();
		expanded.push_back(current);

		if (isGoal(current, goalPos, goalRadius)) {
			frontier.clear();
			buildPath(current);
			break;
		}

		//calculateHookNeighbours(current);

		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				if (x == 0 && y == 0) continue;
				TileNode next;
				next.cell[0] = x + current.cell[0];
				next.cell[1] = y + current.cell[1];
				
				if (inBounds(next.cell[0], next.cell[1])) {
					next.data = tm->getTile(next.cell[0], next.cell[1]);
				}
					
				next.cost = current.cost + cost(current, next);
				h = heuristic(next, goalPos, goalRadius);
				if (updateExpanded(current, next, h) >= expanded.size() && next.cost != INFINITY) {
					next.prev = std::make_shared<TileNode>(current);
					next.heuristic = h;
					frontier.insert(next);
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
		l.setColor(sf::Color::Green);
		l.setThickness(2.0f);
		lines.push_back(l);
	}
	return lines;
}
