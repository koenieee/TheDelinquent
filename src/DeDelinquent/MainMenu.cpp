#include "MainMenu.hpp"
#include "GlobalValues.hpp"
#include "DiskController.hpp"
#include "StateController.hpp"
#include <iostream>
#include "ScreenController.hpp"
#include "DropDownMenu.hpp"



MainMenu::MainMenu(){
	rapidjson::Document * thed = DiskController::readFileIntoJson(MainMenuFilename);
	std::map<std::string, std::string *> * buttons = DiskController::returnJsonObjects(*thed, buttonKeyNames);
	float ySize = (float)(GlobalValues::WINDOW_SIZE_Y) / buttonKeyNames.size() - 50; //solution for window.getSize().y??
	float i = 200;

	for (std::string bText : buttonKeyNames){
		
		if (bText == "CONTINUE_GAME") {
			if (PlayerData::getInstance().getLevelNumber() > 1) {
				std::pair<sf::Text *, sf::RectangleShape *>  * butt = combineButtonAndText(buttons->at(bText)->c_str(), sf::Vector2f{ 5, i }, GlobalValues::WINDOW_SIZE_X - 10); //window.getSize().x
				i += ySize;
				valuesByKeys.insert(std::pair<std::string, std::string>(buttons->at(bText)->c_str(), bText));
				rectusShape.insert(std::pair<sf::Text *, sf::RectangleShape *>(butt->first, butt->second));
			}
		}
		else{
			std::pair<sf::Text *, sf::RectangleShape *>  * butt = combineButtonAndText(buttons->at(bText)->c_str(), sf::Vector2f{ 5, i }, GlobalValues::WINDOW_SIZE_X - 10); //window.getSize().x
			i += ySize;
			valuesByKeys.insert(std::pair<std::string, std::string>(buttons->at(bText)->c_str(), bText));
			rectusShape.insert(std::pair<sf::Text *, sf::RectangleShape *>(butt->first, butt->second));
		}

		
	}
	delete thed;
	thed = nullptr;

	for (std::pair<std::string, std::string *> buttonTexts : *buttons){
		delete buttonTexts.second;
	}
	buttons->clear();
	delete buttons;
	buttons = nullptr;
	
}

MainMenu::~MainMenu(){
	for (std::pair<sf::Text *, sf::RectangleShape *> shape : rectusShape){
		delete shape.first;
		shape.first = nullptr;
		delete shape.second;
		shape.second = nullptr;
	}
	rectusShape.clear();
	delete titleText;
	titleText = nullptr;
}

std::vector<KeyMap::function> MainMenu::getKeyMap(){
	return MainMenu::allTheKeys;
}


void MainMenu::keyPressed(KeyMap::function func){
	if (func == KeyMap::select)
	{
		if (!rectusShape.empty()){
			for (std::pair<sf::Text *, sf::RectangleShape *> thePair : rectusShape){
				if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)InputController::getInstance().getMousePos().x, (float)InputController::getInstance().getMousePos().y })){


					if (valuesByKeys.at((std::string)thePair.first->getString()) == "CONTINUE_GAME"){
						if (PlayerData::getInstance().getLevelNumber() > 1) {
							StateController::setActiveState(StateController::HOME_BASE_MENU);
							return;
						}
					}
					else if (valuesByKeys.at((std::string)thePair.first->getString()) == "NEW_GAME"){
						PlayerData::getInstance().setLevelNumber(1);
						PlayerData::getInstance().resetPlayerDataFile(); //start from level zero.
					//	PlayerData::getInstance().savePlayerDataToFile();
						StateController::drawSplashScreen = false;
						StateController::setActiveState(StateController::LOGIC_CONTROLLER);
						return;
					}
					else if (valuesByKeys.at((std::string)thePair.first->getString()) == "LVL_EDITOR"){
						StateController::drawSplashScreen = false;
						StateController::setActiveState(StateController::LEVEL_EDITOR);
						return;
					}
					else if (valuesByKeys.at((std::string)thePair.first->getString()) == "GAME_EXPLANATION"){
						StateController::setActiveState(StateController::GAME_EXPLANATION);
						return;
					}
					else if (valuesByKeys.at((std::string)thePair.first->getString()) == "EXIT_GAME"){
						ScreenController::shutDown();
						exit(0);
					}
				}
				else{
					if ((thePair.first->getColor() == sf::Color::White && thePair.first->getStyle() == sf::Text::Bold)){ //when it is changed
						thePair.first->setStyle(sf::Text::Regular);
						thePair.first->setColor(sf::Color{ 64, 220, 225 });
					}
				}
			}
		}
	}
}

void MainMenu::mouseHover(sf::Vector2i position){
	for (std::pair<sf::Text *, sf::RectangleShape *> thePair : rectusShape){
		if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)position.x, (float)position.y })){
			//mouse hovered
			if (!(thePair.first->getColor() == sf::Color::White && thePair.first->getStyle() == sf::Text::Bold)){ //when it's not changed
				thePair.first->setColor(sf::Color::White);
				thePair.first->setStyle(sf::Text::Bold);
			}
		}
		else{
			if ((thePair.first->getColor() == sf::Color::White && thePair.first->getStyle() == sf::Text::Bold)){ //when it is changed
				thePair.first->setStyle(sf::Text::Regular);
				thePair.first->setColor(sf::Color{ 64, 220, 225 });
			}
		}
	}
}


void MainMenu::drawScrollable(sf::RenderWindow & window) {
}

void MainMenu::drawFixed(sf::RenderWindow & window){
	State::drawBackground(window);
	for (std::pair<sf::Text *, sf::RectangleShape *> butt : rectusShape){
		if (butt.first != nullptr){
			window.draw(*titleText);
			window.draw(*butt.second);
			window.draw(*butt.first);
		}
	}
}



void MainMenu::update(){}