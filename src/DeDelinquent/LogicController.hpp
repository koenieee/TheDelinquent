#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "InputListener.hpp"
#include "InputController.hpp"
#include "PickupItems.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "PlayerData.hpp"
#include "ExitPoint.hpp"
#include <memory>

/**
* This is used to initialize and control all in game objects such as the Level, Player and the Guards. 
* @version 1.0
*/
class LogicController : public State
{

public:

	LogicController::LogicController();
	LogicController::~LogicController();

	/**
	* Loads a Level selected by levelNumber.
	*
	* @param levelNumber a int value.
	*/
	void LogicController::loadLevelNumber(int levelNumber);

	/**
	* Receives keypresses LogicController wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void LogicController::keyPressed(KeyMap::function func) override;

	/**
	* Draws on a scrollable view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void LogicController::drawScrollable(sf::RenderWindow & window) override;

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void LogicController::drawFixed(sf::RenderWindow & window) override;

	/**
	* Updates the LogicController.
	*/
	void State::update() override;

	/**
	* Updates mouseHovers to LogicController.
	*/
	void LogicController::mouseHover(sf::Vector2i position) override;

	/**
	* Gets the keymap LogicController wants to listen to.
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;

private:

	sf::Font * theFont;
	sf::Text objectiveText;
	int currentLoadLevelNumber = 0;

	sf::Clock clock;
	int textSeconds = 10;

	std::shared_ptr<ExitPoint> exitPoints = nullptr;
	std::shared_ptr<PickupItems> allPickupItems = nullptr;
	std::shared_ptr<Level> theLevel = nullptr;
	std::shared_ptr<Player> player1 = nullptr;

	std::vector<std::shared_ptr<UpdateableComponent>> updatebles;
	std::vector<std::shared_ptr<IDrawableComponent>> drawables;


	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{ KeyMap::select, KeyMap::exit, KeyMap::use };
};