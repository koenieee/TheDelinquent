#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AwarenessMeter.hpp"
#include "GlobalValues.hpp"
/**
GameElementsExplanation is used to display demo examples of game elements
@version 1.0
*/
class GameElementsExplanation : public State
{

public:

	/**
	Constructor the read the splash image into a SplashScreen object

	@return one new SplashScreen object containing the splash Image.
	*/
	GameElementsExplanation::GameElementsExplanation();

	/**
	* Draws on a fixed view.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void drawFixed(sf::RenderWindow&) override;

	/**
	* Updates the GameElementsExplanation.
	*/
	void State::update() override;

	/**
	* Receives keypresses GameElementsExplanation wants to listen to.
	*
	* @param func a KeyMap::function argument.
	*/
	void State::keyPressed(KeyMap::function func) override;

	/**
	* Gets the keymap GameElementsExplanation wants to listen to.
	*/
	std::vector<KeyMap::function> GameElementsExplanation::getKeyMap() override;

private:
	const std::string gameElementsFileName = GlobalValues::MAIN_PATH() + "images/GameElementenInfoScherm.png";
	const std::string playerDemoFileName = GlobalValues::MAIN_PATH() + "images/protagonistTextureAll.png";
	const std::string guardDemoFileName = GlobalValues::MAIN_PATH() + "images/bewakerTextureAll.png";
	const std::string mainDemoFileName = GlobalValues::MAIN_PATH() + "images/MainObjects/LevelOneMainMission.jpg";
	const std::string sideDemoFileName = GlobalValues::MAIN_PATH() + "images/SubObjects/item_2.png";

	sf::Image temp1;
	sf::Texture backgroundImage;
	sf::Sprite loadedBackgroundImage;
	
	sf::Image temp2;
	sf::Texture playerDemoImage;
	sf::Sprite loadedPlayerDemoImage;

	sf::Image temp3;
	sf::Texture guardDemoImage;
	sf::Sprite loadedGuardDemoImage;

	sf::Image temp4;
	sf::Texture mainDemoImage;
	sf::Sprite loadedMainDemoImage;

	sf::Image temp5;
	sf::Texture sideDemoImage;
	sf::Sprite loadedSideDemoImage;


	AwarenessMeter awarenessDemo = AwarenessMeter::getInstance();//AwarenessMeter{};


	int i = 0;
	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{KeyMap::exit};

};
