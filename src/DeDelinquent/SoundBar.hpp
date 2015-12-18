#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include "GlobalValues.hpp"

/**
* The class SoundBar
*
* Defines how the soundbar will be drawn
* and what values it currently contains.
* @version 1.0
*/


class SoundBar{

private:
	/**
	* A private constructor
	*/
	SoundBar();


public:

	/**
	* Get Instance of the SoundBar.
	*
	* @return a SoundBar reference.
	*/
	static SoundBar& getInstance(){

		static SoundBar thisInstance;
		thisInstance.setWindow();
		return thisInstance;
	}

	/**
	* Set the window the SoundBar needs to be drawn to.
	*/
	void setWindow();


	/**
	* Draws all the private RectangleShapes of the soundbar.
	*
	* @param &window, a sf::RenderWindow reference.
	*/
	void drawSoundBar(sf::RenderWindow &window);

	/**
	* Updates the size and position of the SoundIndicator object, also known as the line. 
	*/
	void updateSound();

	/**
	* Maximum value to give the sound multiplier is 1.5; 
	*/
	void setSoundMultiplier(float sm);

	/**
	* Get the Sound Multiplier value
	* 
	* @return a float ranging 0.0f - 1.5f
	*/
	float getSoundMultiplier();


private:

	sf::RectangleShape line, baseLine, soundBar;
	float soundMultiplier = 0.0f, baseLenghtSound = 0.08f, baseWidthSound = 0.1f;


};
