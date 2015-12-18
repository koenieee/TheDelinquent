#include "EndLevel.hpp"
#include "DiskController.hpp"
#include "InputController.hpp"
#include "GlobalValues.hpp"
#include "PlayerData.hpp"
#include <iostream>

EndLevel::EndLevel(bool gameover, bool gamePaused){
	PlayerData::getInstance().playerMayFinish(false);
	float i = 0;
	if (gameover){
		titleText = genText("Game Over!", sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 9 * 9), (float)250 }, 50);
		buttonKeyNames = std::vector<std::string>{ "RESTART_LEVEL", "BACK_TO_BASE" };
		i = 400;
	}
	else{
		titleText = genText("Level Voltooid!", sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 12 * 9), (float)250 }, 50);
		experienceGathered = genText("Ervaring: " + std::to_string(PlayerData::getInstance().getMainObjectiveExp()), sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 12 * 9), (float)350 }, 35);
		nextLevel = genText("Volgend Level: " + std::to_string(PlayerData::getInstance().getLevelNumber()), sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 12 * 9), (float)400 }, 35);
		std::string levelName = GlobalValues::MAIN_PATH() + "levels/level_map_" + std::to_string(PlayerData::getInstance().getLevelNumber()) + ".json";
		std::ifstream infile(levelName);


		if (!infile.good()){ //if file doesn't exists, let the user know the next level is empty.
			enableLevelEditor = genText("Helaas de levels zijn op!\nMaak een level in LevelEditor ", sf::Vector2f{ (((float)GlobalValues::WINDOW_SIZE_X / 2) - 12 * 9), (float)450 }, 35);
			i = 600;
		}
		else{
			i = 500;
		}
		buttonKeyNames = std::vector<std::string>{ "BACK_TO_BASE" };

		
	}
	rapidjson::Document * thed = DiskController::readFileIntoJson(MainMenuFilename);
	std::map<std::string, std::string *> * buttons = DiskController::returnJsonObjects(*thed, buttonKeyNames);

	float ySize = (float)(GlobalValues::WINDOW_SIZE_Y) / buttonKeyNames.size() - 250; //solution for window.getSize().y??
	

	for (std::string bText : buttonKeyNames){

		std::pair<sf::Text *, sf::RectangleShape *>  * butt = combineButtonAndText(buttons->at(bText)->c_str(), sf::Vector2f{ 5, i }, GlobalValues::WINDOW_SIZE_X - 10); //window.getSize().x
		i += ySize;
		valuesByKeys.insert(std::pair<std::string, std::string>(buttons->at(bText)->c_str(), bText));
		buttonShapesAndText.insert(std::pair<sf::Text *, sf::RectangleShape *>(butt->first, butt->second));
	}

	delete thed;
	
}

EndLevel::~EndLevel(){
	for (std::pair<sf::Text *, sf::RectangleShape *> shape : buttonShapesAndText){
		delete shape.first;
		shape.first = nullptr;
		delete shape.second;
		shape.second = nullptr;
	}
	buttonShapesAndText.clear();
	delete titleText;
	titleText = nullptr;

	if (experienceGathered != nullptr){
		delete experienceGathered;
		experienceGathered = nullptr;
		delete nextLevel;
		experienceGathered = nullptr;
	}
	
	
}


std::vector<KeyMap::function> EndLevel::getKeyMap(){
	return EndLevel::allTheKeys;
}


void EndLevel::keyPressed(KeyMap::function func){
	if (KeyMap::select == func){
		if (!buttonShapesAndText.empty()){

			for (std::pair<sf::Text *, sf::RectangleShape *> thePair : buttonShapesAndText){
				if (thePair.second->getGlobalBounds().contains(sf::Vector2f{ (float)InputController::getInstance().getMousePos().x, (float)InputController::getInstance().getMousePos().y })){


					if (valuesByKeys.at((std::string)thePair.first->getString()) == "RESTART_LEVEL"){
						StateController::drawSplashScreen = false;
						StateController::setActiveState(StateController::LOGIC_CONTROLLER); //starting latest player level
						return;
					}
					else if (valuesByKeys.at((std::string)thePair.first->getString()) == "BACK_TO_BASE"){
						StateController::drawSplashScreen = true;
						StateController::setActiveState(StateController::HOME_BASE_MENU); //starting latest player level
						return;
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

void EndLevel::mouseHover(sf::Vector2i position){
	for (std::pair<sf::Text *, sf::RectangleShape *> thePair : buttonShapesAndText){
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

void EndLevel::drawScrollable(sf::RenderWindow & window) {
}

void EndLevel::drawFixed(sf::RenderWindow & window){

	State::drawBackground(window);

	for (std::pair<sf::Text *, sf::RectangleShape *> buttons : buttonShapesAndText){

		if (titleText != nullptr){
			window.draw(*titleText);
		}
		if (experienceGathered != nullptr){
			window.draw(*experienceGathered);
		}
		if (nextLevel != nullptr){
			window.draw(*nextLevel);
		}
		if (enableLevelEditor != nullptr){
			window.draw(*enableLevelEditor);
		}
		window.draw(*buttons.second);
		window.draw(*buttons.first);

	}

	
}



void EndLevel::update(){
//todo


}
