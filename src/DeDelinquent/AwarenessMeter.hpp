#pragma once
#include <SFML\Graphics.hpp>

/**
* Displays an awarenessmeter to the window and updates it when called
* @version 1.1
*/
class AwarenessMeter
{

private:
	/**
	* Initializes the AwarenessMeter based on the window size
	*
	* @param window A sf::RenderWindow reference
	*/
	AwarenessMeter();

public:

	/**
	* Get Instance of the AwarenessMeter.
	*
	* @return a AwarenessMeter reference.
	*/
	static AwarenessMeter& getInstance(){

		static AwarenessMeter thisInstance;
		return thisInstance;
	}

	/**
	* Draws the awarenessmeter to the window.
	* 
	* @param window A sf::RenderWindow reference.
	*/
	void AwarenessMeter::windowUpdate(sf::RenderWindow& window);

	/**
	* Updates the awarenessmeter with a specified value.
	* 
	* @param raw An integer argument ranging from 0 - 255.
	*/
	void AwarenessMeter::awarenessUpdate(int raw);

private:
	const float sizerFactor = 0.000784313f;
	const sf::Uint8 bluenessFactor = 32;
	sf::RectangleShape AwarenessBar;
	sf::RectangleShape border = sf::RectangleShape(sf::Vector2f(0.0f,0.0f));
	sf::RectangleShape divider1, divider2;
};

