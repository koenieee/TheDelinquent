#include "HomeBase.hpp"
#include "GlobalValues.hpp"
#include "DiskController.hpp"
#include "StateController.hpp"

#include <iostream>


HomeBase::HomeBase(){
	PlayerData::getInstance().setDrawPos(sf::Vector2f{ GlobalValues::WINDOW_SIZE_X / 10, 100 });

	rapidjson::Document * thed = DiskController::readFileIntoJson(homeBaseFilename);
	std::map<std::string, std::string *> * buttons = DiskController::returnJsonObjects(*thed, buttonKeyNames);

	float ySize = (float)(GlobalValues::WINDOW_SIZE_Y) / buttonKeyNames.size() - 15; //solution for window.getSize().y??
	float i = 100;

	for (std::string bText : buttonKeyNames){

		std::pair<sf::Text *, sf::RectangleShape *>  * butt = combineButtonAndText(buttons->at(bText)->c_str(), sf::Vector2f{ 5, i }, GlobalValues::WINDOW_SIZE_X - 10); //window.getSize().x
		i += ySize;
		valuesByKeys.insert(std::pair<std::string, std::string>(buttons->at(bText)->c_str(), bText));
		rectangleButtons.insert(std::pair<sf::Text *, sf::RectangleShape *>(butt->first, butt->second));


	}
}

std::vector<KeyMap::function> HomeBase::getKeyMap(){
	return HomeBase::allTheKeys;
}

void HomeBase::keyPressed(KeyMap::function func){

	switch (func)
	{

	case KeyMap::exit:

		StateController::setActiveState(StateController::MAIN_MENU);
		return;


	case KeyMap::select:

		for (std::pair<sf::Text *, sf::RectangleShape *> thePair : rectangleButtons){
			if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)InputController::getInstance().getMousePos().x, (float)InputController::getInstance().getMousePos().y })){

				if (valuesByKeys.at((std::string)thePair.first->getString()) == "START_LEVEL"){
					StateController::drawSplashScreen = false;
					StateController::setActiveState(StateController::LOGIC_CONTROLLER);
					return;
				}
				else if (valuesByKeys.at((std::string)thePair.first->getString()) == "UPGRADE_SPEED"){
					PlayerData::getInstance().increasePlayerSpeed(1);
				}
				else if (valuesByKeys.at((std::string)thePair.first->getString()) == "UPGRADE_STEALTH"){
					PlayerData::getInstance().increaseStealthAbility(1);
				}
				else if (valuesByKeys.at((std::string)thePair.first->getString()) == "SAVE_PLAYER_DATA"){
					PlayerData::getInstance().savePlayerDataToFile();
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

void HomeBase::mouseHover(sf::Vector2i position){
	for (std::pair<sf::Text *, sf::RectangleShape *> thePair : rectangleButtons){
		if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)position.x, (float)position.y })){
			thePair.first->setStyle(sf::Text::Bold);
			thePair.first->setColor(sf::Color::White);
		}
		else{
			thePair.first->setStyle(sf::Text::Regular);
			thePair.first->setColor(sf::Color{ 64, 220, 225 });
		}
	}

}

void HomeBase::drawScrollable(sf::RenderWindow & window) {}

void HomeBase::drawFixed(sf::RenderWindow & window){
	State::drawBackground(window);
	for (std::pair<sf::Text *, sf::RectangleShape *> butt : rectangleButtons){
		window.draw(*titleText);
		
		//drawing playerdata in menu:
		PlayerData::getInstance().draw(window);

		//end drawing playerData

		window.draw(*butt.second);
		window.draw(*butt.first);
	}
}

void HomeBase::update(){}