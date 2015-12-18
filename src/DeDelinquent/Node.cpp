#include "Node.hpp"

Node::Node() {
	dist = std::numeric_limits<double>::infinity();
}


Node::~Node()
{
	parent = nullptr;
	delete parent;
	parent = 0;
}

sf::Vector2f Node::getPos() { return pos; }

double Node::getDistance() { return dist; }

bool Node::isVisited() { return visited; }
	
Node* Node::getParent() { return parent; }

void Node::setPos(sf::Vector2f pos) { Node::pos = pos; }

void Node::setDistance(double dist) { Node::dist = dist; }

void Node::setParent(Node* node) { parent = node; }

void Node::setVisited(bool visited) { Node::visited = visited; }

double Node::distanceBetween(Node* other) {
	return sqrt(pow(pos.x - other->pos.x, 2) + pow(pos.y - other->pos.y, 2));
}