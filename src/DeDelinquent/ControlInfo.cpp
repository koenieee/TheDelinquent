#include "ControlInfo.hpp"

ControlInfo::ControlInfo(){
	
	if (!ControlInfo::temp.loadFromFile(controlInfoImageFilename)){
		throw new std::exception("INFO IMAGE NOT FOUND");
	}

	
	ControlInfo::splashImage.loadFromImage(ControlInfo::temp);
	ControlInfo::loadedSplashImage.setTexture(ControlInfo::splashImage, true);
	ControlInfo::loadedSplashImage.setScale(sf::Vector2f{
		(GlobalValues::WINDOW_SIZE_X / 1024), (GlobalValues::WINDOW_SIZE_Y / 768) });
	ControlInfo::loadedSplashImage.setPosition(sf::Vector2f{ 0, 0 });
	
}

void ControlInfo::update(){}

void ControlInfo::keyPressed(KeyMap::function func){
	switch (func){
	case KeyMap::exit:
		StateController::setActiveState(StateController::GAME_EXPLANATION);
	}
}

void ControlInfo::drawFixed(sf::RenderWindow & window){
	window.draw(ControlInfo::loadedSplashImage);
}

std::vector<KeyMap::function> ControlInfo::getKeyMap(){
	return ControlInfo::allTheKeys;
}