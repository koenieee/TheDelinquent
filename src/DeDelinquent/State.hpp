#pragma once

#include <SFML/Graphics.hpp>
#include "InputListener.hpp"
#include "GlobalValues.hpp"
#include <iostream>

/**
* Provides an interface for States
* @version 1.0
*/
class State: public InputListener {

public:

	/**
	* Draws on a scrollable view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	virtual void State::drawScrollable(sf::RenderWindow & window);

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	virtual void State::drawFixed(sf::RenderWindow & window);

	/**
	* A virtual method to update the Active State.
	*/
	virtual void State::update() = 0;

	/**
	* A virtual method to update key presses for the Active State.
	*/
	virtual void InputListener::keyPressed(KeyMap::function func) override = 0;

	/**
	* Draws the background, called by all states that want a background.
	*
	* @param &window a sf::RenderWindow reference.
	*/
	static void State::drawBackground(sf::RenderWindow & window);

	/**
	* A static method for drawing menu button rectangles.
	* 
	* @param position a sf::Vector2f argument.
	* @param theColor a sf::Color argument.
	* @param sizeRectangle a sf::Vector2f argument.
	* @return a sf::RectangleShape pointer.
	*/
	static sf::RectangleShape * State::drawRect(sf::Vector2f position, sf::Color theColor, sf::Vector2f sizeRectangle);
	

	/**
	* A static method for pairing buttontext with buttonrectangle.
	*
	* @param Text a std::string argument.
	* @param posje a sf::Vector2f argument.
	* @param screenWidth an integer argument.
	* @return std::pair<sf::Text *, sf::RectangleShape *> pointer.
	*/
	static std::pair<sf::Text *, sf::RectangleShape *>  * State::combineButtonAndText(std::string Text, sf::Vector2f posje, int screenWidth);
	
	/**
	* A static method generating drawable text.
	*
	* @param theName a std::string argument.
	* @param pos a sf::Vector2f argument.
	* @param textSize a integer argument.
	* @return sf::Text pointer.
	*/
	static sf::Text * State::genText(std::string theName, sf::Vector2f pos = { 200, 150 }, int TextSize = 30, sf::Color theCol = sf::Color{ 64, 220, 225 });

	/**
	* A static method generating a font.
	*
	* @return a sf::Font pointer.
	*/
	static sf::Font * State::createFont();

	static sf::Font * State::m_font; //initialzed in StateController as nullptr

	/**
	* A method calculating if the mouse is above a button
	*/
	virtual void State::mouseHover(sf::Vector2i position);

	virtual ~State();

	/**
	* A static method loading the background.
	*/
	static void loadBackground();

private:
	static const std::string imageFileName;
	static sf::Image temp;
	static sf::Texture background;
	static sf::Sprite loadedBackground;
};