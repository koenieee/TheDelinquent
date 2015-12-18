#pragma once

#include <SFML/Graphics.hpp>

#include <limits>

/**
* A Node which holds Dijkstra's algorithm specific data
* @version 1.0
*/
class Node
{
public:
	/**
	* Initializes the Node to its default values
	*/
	Node::Node();

	/**
	* Destroys the Node object
	*/
	Node::~Node();

	/**
	* Retrieves the position of the Node
	* @return the position of the Node
	*/
	sf::Vector2f Node::getPos();

	/**
	* Retrieves the distance of the Node
	* @return the distance of the Node
	*/
	double Node::getDistance();

	/**
	* Retrieves if the Node was already visited
	* @return the visited status
	*/
	bool Node::isVisited();
	
	/**
	* Retrieves the parent
	* @return the parent, nullptr if it has none
	*/
	Node* Node::getParent();

	/**
	* Sets the position of the Node
	* @param pos the new position
	*/
	void Node::setPos(sf::Vector2f pos);
	
	/**
	* Sets the distance of the Node
	* @param dist the new distance
	*/
	void Node::setDistance(double dist);

	/**
	* Sets the parent of the Node
	* @param node the new parent
	*/
	void Node::setParent(Node* node);

	/**
	* Sets the visited status of the Node
	* @param visited the visited status
	*/
	void Node::setVisited(bool visited);

	/**
	* Calculates the distance between this Node and another
	* @param other the Node to compare to
	* @return the distance between both nodes
	*/
	double Node::distanceBetween(Node* other);

	/**
	* Holds the index for easy indexing in Grid
	*/
	sf::Vector2u index;

private:
	sf::Vector2f pos;
	double dist;
	bool visited = false;
	Node* parent = nullptr;
};
