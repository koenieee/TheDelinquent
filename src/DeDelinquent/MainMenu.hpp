#pragma once

#include "State.hpp"
#include "InputListener.hpp"
#include "GlobalValues.hpp"
#include "InputController.hpp"
#include <unordered_map>
#include "PlayerData.hpp"
#include "ScreenController.hpp"


/**
* Allows for navigation through the main menu
* @version 1.0
*/
class MainMenu : public State {
public:
	
	MainMenu::MainMenu();

	MainMenu::~MainMenu();

	/**
	* Receives keypresses MainMenu wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void MainMenu::keyPressed(KeyMap::function func) override;

	/**
	* Draws on a scrollable view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void MainMenu::drawScrollable(sf::RenderWindow & window) override;

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void MainMenu::drawFixed(sf::RenderWindow & window) override;

	/**
	* Updates the MainMenu.
	*/
	void State::update() override;

	/**
	* Updates mouseHovers to MainMenu.
	*/
	void MainMenu::mouseHover(sf::Vector2i position) override;

	/**
	* Gets the keymap MainMenu wants to listen to.
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;

private:
	std::vector<std::string> buttonKeyNames = std::vector<std::string>{ "CONTINUE_GAME", "NEW_GAME", "LVL_EDITOR", "GAME_EXPLANATION", "EXIT_GAME" };
	std::string MainMenuFilename = std::string{ GlobalValues::MAIN_PATH() + "json_data/MainMenu_nl.json" };
	std::unordered_map< sf::Text *, sf::RectangleShape *> rectusShape;
	std::map<std::string, std::string> valuesByKeys;
	sf::Text * titleText = genText("De Delinquent", sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 13 * 9), (float)50 }, 50, sf::Color{ 0, 255, 251 });
	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{KeyMap::select};
};