#pragma once

#include "State.hpp"
#include "InputListener.hpp"
#include "StateController.hpp"

/**
* Class for in game menu
* @version 1.0
*/

class EndLevel : public State {
public:

	/**
	* A constructor to configure the ingame menu.
	*/
	EndLevel::EndLevel(bool gameover, bool gamePaused = false); //set to true if player is GameOver, set to false to display Scoreboard

	EndLevel::~EndLevel();

	/**
	* Receives keypresses InGameMenu wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void EndLevel::keyPressed(KeyMap::function func) override;
	
	/**
	* Draws on a scrollable view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void EndLevel::drawScrollable(sf::RenderWindow & window) override;

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void EndLevel::drawFixed(sf::RenderWindow & window) override;

	/**
	* Updates the InGameMenu.
	*/
	void State::update() override;

	/**
	* Updates mouseHovers to InGameMenu.
	*/
	void EndLevel::mouseHover(sf::Vector2i position) override;



	std::vector<KeyMap::function> InputListener::getKeyMap() override;

private:


	std::vector<std::string> buttonKeyNames;
	std::map<std::string, std::string> valuesByKeys;

	std::string MainMenuFilename = std::string{ GlobalValues::MAIN_PATH() + "json_data/IngameMenu_nl.json" };

	sf::Text * experienceGathered = nullptr;
	sf::Text * nextLevel = nullptr;
	sf::Text * titleText = nullptr;
	sf::Text * enableLevelEditor = nullptr;

	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{ KeyMap::select, KeyMap::pause };

	std::map<sf::Text *, sf::RectangleShape *> buttonShapesAndText;
		
};