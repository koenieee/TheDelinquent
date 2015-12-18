#include "GameElementsExplanation.hpp"
#include <iostream>
#include "GlobalValues.hpp"
#include "StateController.hpp"

GameElementsExplanation::GameElementsExplanation()
{

	//loading all images from disk
	if (!GameElementsExplanation::temp1.loadFromFile(gameElementsFileName)){
		throw new std::exception("BACKGROUND IMAGE NOT FOUND");
	}
	if (!GameElementsExplanation::temp2.loadFromFile(playerDemoFileName)){
		throw new std::exception("PLAYER IMAGE NOT FOUND");
	}
	if (!GameElementsExplanation::temp3.loadFromFile(guardDemoFileName)){
		throw new std::exception("GUARD IMAGE NOT FOUND");
	}
	if (!GameElementsExplanation::temp4.loadFromFile(mainDemoFileName)){
		throw new std::exception("MAIN IMAGE NOT FOUND");
	}
	if (!GameElementsExplanation::temp5.loadFromFile(sideDemoFileName)){
		throw new std::exception("SIDE IMAGE NOT FOUND");
	}

	//setting all textures
	GameElementsExplanation::backgroundImage.loadFromImage(temp1);
	GameElementsExplanation::playerDemoImage.loadFromImage(temp2);
	GameElementsExplanation::guardDemoImage.loadFromImage(temp3);
	GameElementsExplanation::mainDemoImage.loadFromImage(temp4);
	GameElementsExplanation::sideDemoImage.loadFromImage(temp5);

	//setting all sprites
	GameElementsExplanation::loadedBackgroundImage.setTexture(GameElementsExplanation::backgroundImage, true);
	GameElementsExplanation::loadedPlayerDemoImage.setTexture(GameElementsExplanation::playerDemoImage, true);
	GameElementsExplanation::loadedGuardDemoImage.setTexture(GameElementsExplanation::guardDemoImage, true);
	GameElementsExplanation::loadedMainDemoImage.setTexture(GameElementsExplanation::mainDemoImage, true);
	GameElementsExplanation::loadedSideDemoImage.setTexture(GameElementsExplanation::sideDemoImage, true);

	//configuring styles and positions
	GameElementsExplanation::loadedBackgroundImage.setScale(sf::Vector2f{
		(GlobalValues::WINDOW_SIZE_X / 1024), (GlobalValues::WINDOW_SIZE_Y / 768) });
	GameElementsExplanation::loadedPlayerDemoImage.setScale(sf::Vector2f{ 1.14f, 1.14f });
	GameElementsExplanation::loadedGuardDemoImage.setScale(sf::Vector2f{ 1.14f, 1.14f });
	GameElementsExplanation::loadedPlayerDemoImage.setTextureRect(sf::IntRect{ 43 * (GameElementsExplanation::i / 64), 0, 43, 43 });
	GameElementsExplanation::loadedGuardDemoImage.setTextureRect(sf::IntRect{ 43 * (GameElementsExplanation::i / 64), 0, 43, 43 });
	GameElementsExplanation::loadedBackgroundImage.setPosition(sf::Vector2f{ 0, 0 });
	GameElementsExplanation::loadedPlayerDemoImage.setPosition(sf::Vector2f{ 90, 70 });
	GameElementsExplanation::loadedGuardDemoImage.setPosition(sf::Vector2f{ 90, 260 });
	GameElementsExplanation::loadedMainDemoImage.setPosition(sf::Vector2f{ 900, 40 });
	GameElementsExplanation::loadedSideDemoImage.setPosition(sf::Vector2f{ 900, 230 });
}

void GameElementsExplanation::drawFixed(sf::RenderWindow& window){
	window.draw(GameElementsExplanation::loadedBackgroundImage);
	window.draw(GameElementsExplanation::loadedPlayerDemoImage);
	window.draw(GameElementsExplanation::loadedGuardDemoImage);
	window.draw(GameElementsExplanation::loadedMainDemoImage);
	window.draw(GameElementsExplanation::loadedSideDemoImage);
	awarenessDemo.windowUpdate(window);
}

void GameElementsExplanation::update(){
	GameElementsExplanation::i = (GameElementsExplanation::i + 1) % 512;
	GameElementsExplanation::awarenessDemo.awarenessUpdate(GameElementsExplanation::i/2);
	GameElementsExplanation::loadedPlayerDemoImage.setTextureRect(sf::IntRect{ 43 * (GameElementsExplanation::i / 64), 0, 43, 43 });
	GameElementsExplanation::loadedGuardDemoImage.setTextureRect(sf::IntRect{ 43 * ((GameElementsExplanation::i / 64 + 4) % 8), 0, 43, 43 });


}

void GameElementsExplanation::keyPressed(KeyMap::function func){
	if (func == KeyMap::exit){
		StateController::setActiveState(StateController::GAME_EXPLANATION);
	}
}

std::vector<KeyMap::function> GameElementsExplanation::getKeyMap(){
	return GameElementsExplanation::allTheKeys;
}