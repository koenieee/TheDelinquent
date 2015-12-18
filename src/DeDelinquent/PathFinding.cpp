#include "PathFinding.hpp"


PathFinding::PathFinding(std::shared_ptr<Level> l, unsigned int interval, unsigned int playerSize) : g{ l,interval,playerSize }
{
}
PathFinding::~PathFinding()
{
}

std::vector<sf::Vector2f> PathFinding::calculatePath(sf::Vector2f start, sf::Vector2f end) {
	// Check if direct path is possible

		
	// If direct path is unavailable, use Dijkstra
	return calculatePath(g.findNearest(start), g.findNearest(end));
}

std::vector<sf::Vector2f> PathFinding::calculatePath(Node* start, Node* end) {
	if (start == nullptr || end == nullptr) {
		throw std::exception{ "Invalid Nodes!" };
	}


	Grid copy{ g }; // Copy the Grid so no modifications will be made to the source
	Node* current = copy.getNodeAt(start->index.x, start->index.y); // initial node
	end = copy.getNodeAt(end->index.x, end->index.y);

	// Set the initial Node to visited and the distance to 0
	current->setVisited(true);
	current->setDistance(0);

	// Fill unvisited list of nodes
	std::set<Node*> unvisited;

	while (current != nullptr) {
		// Check all neighbours
		std::vector<Node*> neighbours = copy.getNeighbours(current->index.x, current->index.y);// (unsigned int)vi.x, (unsigned int)vi.y);
		for (auto n : neighbours) {
			double dist = current->distanceBetween(n) + current->getDistance();
			if (!n->isVisited()) {
				unvisited.emplace(n);
			}
			// If the total distance calculated is smaller
			// than the previously calculated distance, assign it and set the parent
			if (dist < n->getDistance()) {
				n->setDistance(dist);
				n->setParent(current);
			}
		}
		// Set the current Node to visited
		current->setVisited(true);
		// If it is the destination, no more calculations are made
		if (current->index == end->index && current->isVisited())
		{
			break;
		}

		// Remove current from unvisited
		for (std::set<Node*>::iterator it = unvisited.begin(); it != unvisited.end(); it++) {
			if (it._Ptr->_Myval == current) {
				unvisited.erase(it);
				break;
			}
		}

		//Node* leastDist = g.firstUnvisited();
		//if (leastDist == nullptr) {
		//	break;
		//}
		// If no more unvisited nodes exist, stop
		if (unvisited.empty()) {
			break;
		}
		// Find the nearest node to use next
		Node* leastDist = nullptr;
		double least = std::numeric_limits<double>::infinity();
		for (auto n : unvisited) {
			if (n->getDistance() <= least) {
				leastDist = n;
				least = n->getDistance();
			}
		}
		current = leastDist;
	}


	std::vector<sf::Vector2f> path;

	if (current != end) {
		throw std::exception{ "No Path Found!" };
	}

	// Traverse through the node and its parents
	// creating a reverse route
	Node* node = current;
	while (node != nullptr) {
		path.push_back({ node->getPos().x, node->getPos().y });
		node = node->getParent();
	}

	return path;
}

std::vector<sf::Vector2f> PathFinding::getGrid() {
	std::vector<sf::Vector2f> list;
	for (unsigned int i = 0; i < g.getWidth(); i++)
	{
		for (unsigned int j = 0; j < g.getHeight(); j++)
		{
			Node* n = g.getNodeAt(i, j);
			if (!n->isVisited()) {
				list.push_back(n->getPos());
			}
		}
	}
	return list;
}