#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>


/**
* This is just a virtual class to force the definition of overlappingPosition function. 
* @version 1.0
*/
class UnpenetrableComponent
{
public:
	/**
	* Function to determine if the specified FloatRect is overlapping with the components that implements this function.                                                                     
	* @param unpenetrable The unpenetrable component you want to check the overlapping position.
	* @return True or false according to the implementation of the function.
	*/
	virtual bool UnpenetrableComponent::overlappingPosition(sf::FloatRect unpenetrable) = 0;
};