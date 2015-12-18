#include "Grid.hpp"


Grid::Grid(std::shared_ptr<Level> l, unsigned int interval, unsigned int playerSize) {
	// Load a copy of the level mapObjectItems for generation of the Grid
	std::vector<Level::mapObjectItem>* list = l->getAllSprites();
	if (list->empty()) {
		return; // TODO: raise exception?
	}
	sf::FloatRect bounds = list->front().objectSprite.getGlobalBounds();
	sf::Vector2f start = { bounds.left, bounds.top };
	sf::Vector2f end = { bounds.left + bounds.width, bounds.top + bounds.height };

	unsigned int smallestDistanceBetween = 0xFFFFFFFF; // maximum uint value
	unsigned int halfPlayerSize = playerSize / 2 + playerSize % 2;
	// Determine the top-left and bottom-right, based on texture coordinates
	for (auto item : *list) {
		sf::FloatRect temp = item.objectSprite.getGlobalBounds();

		// Check the smallest width or height to use as interval (distance between Nodes)
		if (temp.height <= smallestDistanceBetween) {
			smallestDistanceBetween = temp.height;
		}
		if (temp.width <= smallestDistanceBetween) {
			smallestDistanceBetween = temp.width;
		}
		// Increase size to make sure Guard doesn't get stuck in walls
		temp.left -= halfPlayerSize;
		temp.top -= halfPlayerSize;
		temp.width += playerSize;
		temp.height += playerSize;

		if (temp.left <= start.x) {
			start.x = temp.left;
		}
		if (temp.top <= start.y) {
			start.y = temp.top;
		}
		if (temp.left + temp.width >= end.x) {
			end.x = temp.left + temp.width;
		}
		if (temp.top + temp.height >= end.y) {
			end.y = temp.top + temp.height;
		}

	}
		
	// If the parameter interval wasn't specified, use smallestDistanceBetween
	Grid::interval = interval == 0xFFFFFFFF ? smallestDistanceBetween : interval;

	// Round the width and size appropiately for the distance
	// Height and width are the size of the matrix (vector of vectors)
	width = (unsigned int)roundf((end - start).x / (float)Grid::interval);
	height = (unsigned int)roundf((end - start).y / (float)Grid::interval);

	// Resize and fill the matrix with default values
	fillMatrix();

	// Update positions used to determine fastest route
	for (unsigned int x = 0; x < matrixOfNodes.size(); x++) {
		for (unsigned int y = 0; y < matrixOfNodes[x].size(); y++) {
			matrixOfNodes[x][y].setPos(sf::Vector2f{ (float)x * Grid::interval,(float)y * Grid::interval } + start);
			matrixOfNodes[x][y].index = { x,y };
		}
	}

	std::vector<sf::FloatRect> solidObjs;

	// Check each mapObject for collision
	for (auto item : *list) {
		if (item.collision) {
			solidObjs.push_back(item.objectSprite.getGlobalBounds());
		}
	}
	// Slightly expensive function to disable nodes in solid objects
	sf::FloatRect rect;
	for (auto & item : solidObjs) {
		for (unsigned int x = 0; x < width; ++x) {
			for (unsigned int y = 0; y < height; ++y) {
				rect = item;
				// Increase size to make sure Guard doesn't get stuck in walls
				rect.left -= halfPlayerSize;
				rect.top -= halfPlayerSize;
				rect.width += playerSize;
				rect.height += playerSize;
				if (!matrixOfNodes[x][y].isVisited() && rect.contains(matrixOfNodes[x][y].getPos())) {
					matrixOfNodes[x][y].setVisited(true);
				}
			}
		}
	}
}

Grid::~Grid()
{
}

Node* Grid::findNearest(sf::Vector2f pos) {
	// TODO: round coordinates to unvisited grid coordinates
	if (matrixOfNodes.empty()) {
		return nullptr; // TODO: raise exception?
	}
	// Points that are less than half of an interval away from a node will be returned
	sf::Vector2f start = matrixOfNodes.front().front().getPos(); // top-left
	sf::Vector2f end = matrixOfNodes.back().back().getPos(); // bottom-right
	int roundDown = (int)interval / 2;
	int roundUp = roundDown + interval % 2; // if exactly between round up
	// Check bounds
	if (pos.x < start.x - roundUp
		|| pos.y < start.y - roundUp
		|| pos.x > end.x + roundDown
		|| pos.y > end.y + roundDown) {
		return nullptr; // TODO: raise exception?
	}
	// Positions for the iteration
	int pX = start.x - roundUp; // offset it slightly so it is big enough to 'catch' the coordinate
	int pY = start.y - roundUp;
	for (unsigned int x = 0; x < matrixOfNodes.size(); x++) {
		for (unsigned int y = 0; y < matrixOfNodes[x].size(); y++) {
			if (pos.x >= pX
				&& pos.y >= pY
				&& pos.x <= pX + (int)interval
				&& pos.y <= pY + (int)interval) {
				// found it
				return &matrixOfNodes[x][y];
			}
			pY += interval;
		}
		pY = start.y - roundUp; // Reset to initial position
		pX += interval;
	}
		
	// Raise exception if not found?
	return nullptr;
}

std::vector<Node*> Grid::getNeighbours(unsigned int x, unsigned int y) {
	std::vector<Node*> list;
	for (int _x = -1; _x <= 1; _x++) {
		for (int _y = -1; _y <= 1; _y++) {
			if (_x == 0 && _y == 0) {
				continue; // skip middle, it's the node itself
			}
			try
			{
				Node* n = &matrixOfNodes.at(x + _x).at(y + _y);
				if (!n->isVisited()) {
					list.push_back(n);
				}
			}
			catch (const std::exception&) // exception raised by at() bounds check
			{
			}
		}
	}
	return list;
}

unsigned int Grid::getWidth() { return width; }
unsigned int Grid::getHeight() { return height; }

Node* Grid::getNodeAt(unsigned int x, unsigned int y) {
	if (x > width || y > height) {
		return nullptr; // TODO: raise exception
	}
	return &matrixOfNodes[x][y];
}



void Grid::fillMatrix() {
	matrixOfNodes.resize(width);
	for (auto& n : matrixOfNodes) {
		n.resize(height);
	}

	// TODO: move to constructor?
	for (unsigned int x = 0; x < matrixOfNodes.size(); x++) {
		for (unsigned int y = 0; y < matrixOfNodes[x].size(); y++) {
			matrixOfNodes[x][y].index = { x,y };
		}
	}
}