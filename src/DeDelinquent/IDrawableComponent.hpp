#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>

/**
* Provides methods for a single drawable component
* @version 1.0
*/
class IDrawableComponent{
public:

	/**
	* Destructor of the IDrawableComponent component
	*/
	virtual IDrawableComponent::~IDrawableComponent(){};

	/**
	* Draws the component on the given window
	* @param window = where the it will be drawn
	*/
	virtual void IDrawableComponent::draw(sf::RenderWindow & window) = 0;

protected:
	sf::Vector2f pos;
	bool drawActive = false;

};