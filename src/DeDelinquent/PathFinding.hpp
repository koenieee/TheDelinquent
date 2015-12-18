#pragma once

#include "Grid.hpp"
#include "Node.hpp"
#include "Level.hpp"
#include <set>
#include <memory>
#include <SFML/Graphics.hpp>

/**
* Uses Dijkstra's algorithm for pathfinding
* @version 1.0
*/
class PathFinding
{
public:


	/**
	* Initializes pathfinding
	* @param l the level to collect solid objects
	* @param interval the distance between points
	* @param playerSize the size of an entity to use
	*/
	PathFinding::PathFinding(std::shared_ptr<Level> l, unsigned int interval = 0xFFFFFFFF, unsigned int playerSize = 46);
	
	/**
	* Destroys the PathFinding object
	*/
	PathFinding::~PathFinding();

	/**
	* Calculates the path from one point to another
	* @param start the starting position
	* @param end the end position
	* @throws std::exception if no path is possible
	* return the calculated path
	*/
	std::vector<sf::Vector2f> PathFinding::calculatePath(sf::Vector2f start, sf::Vector2f end);

	/**
	* Retrieves the positions of all Nodes in the Grid
	* Should be used for testing/rendering
	* @return the positions of all Nodes in the Grid
	*/
	std::vector<sf::Vector2f> PathFinding::getGrid();

private:

	/**
	* Calculates the path from one node to another
	* @param start the starting node
	* @param end the end node
	* @throws std::exception if no path is possible
	* return the calculated path
	*/
	std::vector<sf::Vector2f> PathFinding::calculatePath(Node* start, Node* end);

	Grid g;
};
