#pragma once

#include "State.hpp"
#include "InputListener.hpp"
#include "PlayerData.hpp"
#include "GlobalValues.hpp"
#include <unordered_map>

/**
* Class for home base menu
* @version 1.0
*/

class HomeBase : public State {
public:

	HomeBase::HomeBase();
	
	/**
	* Receives keypresses HomeBase wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void HomeBase::keyPressed(KeyMap::function func) override;
	
	/**
	* Draws on a scrollable view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void HomeBase::drawScrollable(sf::RenderWindow & window) override;

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void HomeBase::drawFixed(sf::RenderWindow & window) override;
	
	/**
	* Updates the HomeBase.
	*/
	void State::update() override;

	/**
	* Updates mouseHovers to HomeBase.
	*/
	void HomeBase::mouseHover(sf::Vector2i position) override;

	/**
	* Gets the keymap HomeBase wants to listen to.
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;


private:
	std::vector<std::string> buttonKeyNames = std::vector<std::string>{ "START_LEVEL", "UPGRADE_SPEED", "UPGRADE_STEALTH", "SAVE_PLAYER_DATA", "BACK_TO_MAIN" }; //json key names
	std::string homeBaseFilename = std::string{ GlobalValues::MAIN_PATH() + "json_data/HomeBase_nl.json" }; //filename for json data
	std::unordered_map< sf::Text *, sf::RectangleShape *> rectangleButtons;
	std::map<std::string, std::string> valuesByKeys;
	sf::Text * titleText = genText("Thuis Basis", sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 11 * 9), (float)10 }, 50, sf::Color{ 0, 255, 251 });
	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{ KeyMap::select, KeyMap::exit };
};

