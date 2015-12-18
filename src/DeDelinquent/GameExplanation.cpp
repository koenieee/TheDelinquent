#include "GameExplanation.hpp"
#include "GlobalValues.hpp"
#include "DiskController.hpp"
#include "StateController.hpp"
#include <iostream>

GameExplanation::GameExplanation(){
	rapidjson::Document * thed = DiskController::readFileIntoJson(MainMenuFilename);
	std::map<std::string, std::string *> * buttons = DiskController::returnJsonObjects(*thed, buttonKeyNames);

	float ySize = (float)(GlobalValues::WINDOW_SIZE_Y) / buttonKeyNames.size() - 120; //solution for window.getSize().y??
	float i = 250;

	for (std::string bText : buttonKeyNames){
		
		std::pair<sf::Text *, sf::RectangleShape *>  * butt = combineButtonAndText(buttons->at(bText)->c_str(), sf::Vector2f{ 5, i }, GlobalValues::WINDOW_SIZE_X - 10); //window.getSize().x
		i += ySize;
		valuesByKeys.insert(std::pair<std::string, std::string>(buttons->at(bText)->c_str(), bText));
		rectusShape.insert(std::pair<sf::Text *, sf::RectangleShape *>(butt->first, butt->second));
	}
}

std::vector<KeyMap::function> GameExplanation::getKeyMap(){
	return GameExplanation::allTheKeys;
}

void GameExplanation::keyPressed(KeyMap::function func){

	switch (func)
	{

	case KeyMap::exit:
		StateController::setActiveState(StateController::MAIN_MENU);
		return;

	case KeyMap::select:

		for (std::pair<sf::Text *, sf::RectangleShape *> thePair : rectusShape){
			if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)InputController::getInstance().getMousePos().x, (float)InputController::getInstance().getMousePos().y })){

				if (valuesByKeys.at((std::string)thePair.first->getString()) == "KEYBOARD_SETTINGS"){
					StateController::setActiveState(StateController::CONTROLS_INFO_SCREEN);
					return;
				}
				else if (valuesByKeys.at((std::string)thePair.first->getString()) == "LEVEL_EXPLANATION"){

					StateController::setActiveState(StateController::GAME_ELEMENTS_INFO_SCREEN);
					return;
				}
				else if (valuesByKeys.at((std::string)thePair.first->getString()) == "BACK_TO_MAIN"){
					StateController::setActiveState(StateController::MAIN_MENU);
					return;
				}
			}
			else{
				thePair.first->setStyle(sf::Text::Regular);
				thePair.first->setColor(sf::Color{ 64, 220, 225 });
			}
		}
	}
}

void GameExplanation::mouseHover(sf::Vector2i position){
	for (std::pair<sf::Text *, sf::RectangleShape *> thePair : rectusShape){
		if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)position.x, (float)position.y })){
			thePair.first->setColor(sf::Color::White);
			thePair.first->setStyle(sf::Text::Bold);
		}
		else{
			thePair.first->setStyle(sf::Text::Regular);
			thePair.first->setColor(sf::Color{ 64, 220, 225 });
		}
	}
}


void GameExplanation::drawScrollable(sf::RenderWindow & window) {}

void GameExplanation::drawFixed(sf::RenderWindow & window){
	State::drawBackground(window);
	for (std::pair<sf::Text *, sf::RectangleShape *> butt : rectusShape){
		window.draw(*titleText);
		window.draw(*butt.second);
		window.draw(*butt.first);
		
	}
	
}

void GameExplanation::update(){}