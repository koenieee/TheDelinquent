#include "InputController.hpp"
#include <SFML/System.hpp>
#include <iostream>
#include <algorithm>
#include "ScreenController.hpp"



InputController::InputController(){
	KeyMap = KeyMap::KeyMap();
	changingMap = false;
}

void InputController::setWindow(sf::Window* gameWindow){
	window = gameWindow;
}


//this even works when the window is not selected
bool InputController::getfunction(KeyMap::function func){
	if (sf::Keyboard::isKeyPressed(KeyMap.keyBoundList[func])){
		return true;
	}
	if (sf::Mouse::isButtonPressed(KeyMap.mouseBoundList[func])){
		return true;
	}
	return false;
}

sf::Vector2i InputController::getMousePos(){
	return sf::Mouse::getPosition(*window);

}

void InputController::addFunctionListener(InputListener * listener){
	if (listener != nullptr) //double check
	{
		while (changingMap){}
		changingMap = true;
		saveEvent.clear();
		functionAndListener fl;
		fl.allTheKeys = std::vector<KeyMap::function>(listener->getKeyMap());
		fl.listener = listener;

		functionEvent.push_back(fl);
		changingMap = false;
	}
}

void InputController::removeFunctionListener(InputListener* listener){

	while (changingMap){}
	changingMap = true;
	saveEvent.clear();
	functionEvent.erase(
		std::remove_if(functionEvent.begin(), functionEvent.end(),
		[&](functionAndListener & item) {
			if (item.listener == listener){
				item.listener = nullptr;
				return true;
			}
			else{
				return false;
			}
		}

		),
		functionEvent.end()
	);
	
	changingMap = false;

	
}

void InputController::update(){

	saveEvent.clear();
	auto itr = functionEvent.begin();
	while (itr != functionEvent.end()) {
		for (const KeyMap::function currentKey : itr->allTheKeys){
			if (getfunction(currentKey) && itr->listener != nullptr && changingMap == false){
				keyIsPressed kis;
				
				kis.pressedKey = currentKey;
				kis.listener = itr->listener;
				saveEvent.push_back(kis);
			}
		}
		++itr;
	}

	for (auto & it : saveEvent) {
		if (!changingMap) {
			if (it.listener != nullptr) {
				it.listener->keyPressed(it.pressedKey);
				if (saveEvent.empty()) { // Check for state transition
					return;
				}
			}
		}
	}
	saveEvent.clear();

}
