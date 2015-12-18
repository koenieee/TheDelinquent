#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>

/**
* Provides an interface for updateable components
* @version 1.0
*/
class UpdateableComponent{
public:

	/**
	* Destructor of the Updateable component
	*/
	virtual UpdateableComponent::~UpdateableComponent() {};

	/**
	* This function will be called every game tick and can be used to update parts of the Object that implements this.
	*/
	virtual void UpdateableComponent::update() = 0;

};
