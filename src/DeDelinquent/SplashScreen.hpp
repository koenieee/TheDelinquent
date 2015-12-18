#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"

/**
* SplashScreen is used to display the HayFase team logo
* @version 1.0
*/
class SplashScreen : public State
{

public:

	/**
	* Loads background and starts SplashScreen.
	*/
	SplashScreen::SplashScreen();

	/**
	* Draws the SplashScreen at full window size.
	*
	* @param the sf::RenderWindow used to draw the splashscreen.
	*/
	void SplashScreen::drawFixed(sf::RenderWindow&) override;

	/**
	* Updates the SplashScreen.
	*/
	void State::update() override;

	/**
	* Updates keypresses for SplashScreen.
	*/
	void InputListener::keyPressed(KeyMap::function func) override;

	/**
	* Gets the keymap SplashScreen wants to listen to.
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;

private:

	sf::Clock clock;
	sf::Time startTime;
	int seconds = 3;

	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{KeyMap::exit};

};
