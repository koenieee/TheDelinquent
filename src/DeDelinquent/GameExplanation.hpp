#pragma once

#include "State.hpp"
#include "InputListener.hpp"
#include "InputController.hpp"
#include "GlobalValues.hpp"

/**
* Allows for configuring the settings
* @version 1.0
*/
class GameExplanation : public State {
public:
	GameExplanation::GameExplanation();

	/**
	* Receives keypresses GameExplanation wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void GameExplanation::keyPressed(KeyMap::function func) override;
	
	/**
	* Draws on a scrollable view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void GameExplanation::drawScrollable(sf::RenderWindow & window) override;

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void GameExplanation::drawFixed(sf::RenderWindow & window) override;

	/**
	* Updates the GameExplanation.
	*/
	void State::update() override;

	/**
	* Updates mouseHovers to GameExplanation.
	*/
	void GameExplanation::mouseHover(sf::Vector2i position) override;
	
	/**
	* Gets the keymap GameExplanation wants to listen to.
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;

private:
	std::vector<std::string> buttonKeyNames = std::vector<std::string>{  "KEYBOARD_SETTINGS", "LEVEL_EXPLANATION", "BACK_TO_MAIN" };
	std::string MainMenuFilename = std::string{ GlobalValues::MAIN_PATH() + "json_data/GameExplanation_nl.json" };
	std::map<sf::Text *, sf::RectangleShape *> rectusShape;
	std::map<std::string, std::string> valuesByKeys;

	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{ KeyMap::select, KeyMap::exit };

	sf::Text * titleText = genText("Spel Uitleg", sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 11 * 9), (float)20 }, 50, sf::Color{ 0, 255, 251 });
};