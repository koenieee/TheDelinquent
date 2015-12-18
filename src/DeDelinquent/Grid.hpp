#pragma once

#include "Node.hpp"
#include "Level.hpp"
#include <vector>
#include <set>
#include <unordered_map>
#include <array>
#include <memory>

/**
* Contains a matrix of Nodes and allows for the retrieval of neighbouring Nodes
* @version 1.0
*/
class Grid
{
public:

	/**
	* Creates a grid of Nodes with a specified interval and player size
	* @param l the level to retrieve solid objects from
	* @param interval the distance between Nodes
	* If not specified, it is calculated based on the level's smallest width or height of the textures
	* @param playerSize the size of the entity to use
	*/
	Grid::Grid(std::shared_ptr<Level> l, unsigned int interval = 50, unsigned int playerSize = 46);

	/**
	* Destroys the Grid
	*/
	Grid::~Grid();

	/**
	* Retrieves the Node nearest to the coordinates provided
	* @param pos the position to be checked
	* @return the nearest Node
	*/
	Node* Grid::findNearest(sf::Vector2f pos);

	/**
	* Retrieves the unvisited neighbours of a node
	* @param x the x-index of the node
	* @param y the y-index of the node
	*/
	std::vector<Node*> Grid::getNeighbours(unsigned int x, unsigned int y);



	/**
	* Returns the width of the grid
	* @return the width of the grid
	*/
	unsigned int Grid::getWidth();

	/**
	* Returns the height of the grid
	* @return the height of the grid
	*/
	unsigned int Grid::getHeight();

	/**
	* Retrieves a Node at a specified index
	* @param x the x index
	* @param y the y index
	* @return the node found, nullptr if parameters are invalid
	*/
	Node* Grid::getNodeAt(unsigned int x, unsigned int y);;



private:
	/*
	* Fills the matrix
	*/
	void Grid::fillMatrix();

	/*
	* The distance between Nodes
	*/
	unsigned int interval;

	std::vector<std::vector<Node>> matrixOfNodes;
	unsigned int width;
	unsigned int height;
};