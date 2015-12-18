#pragma once

#include "SFML\Graphics.hpp"
#include "State.hpp"
#include "StateController.hpp"
#include "InputListener.hpp"
#include "GlobalValues.hpp"

class ControlInfo : public State{

public:

	/**
	* Initializes this object.                                                                     
	*/
	ControlInfo::ControlInfo();

	/**
	* Updates the ControlInfo.
	*/
	void State::update() override;

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void ControlInfo::drawFixed(sf::RenderWindow & window) override;

	/**
	* Receives keypresses ControlInfo wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void State::keyPressed(KeyMap::function func) override;

	/**
	* Gets the keymap ControlInfo wants to listen to.
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;

private:
	const std::string controlInfoImageFilename = GlobalValues::MAIN_PATH() + "images/BesturingsInfoScherm.png";
	sf::Image temp;
	sf::Texture splashImage;
	sf::Sprite loadedSplashImage;
	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{ KeyMap::exit };
};