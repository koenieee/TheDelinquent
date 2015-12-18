// file main.cpp

#include <SFML/Graphics.hpp>
#include "DiskController.hpp"
#include "rapidjson/document.h"
#include "StateController.hpp"
#include <map>
#include <string>
#include <vector>
#include "MainMenu.hpp"
#include "GameExplanation.hpp"
#include "EndLevel.hpp"
#include "HomeBase.hpp"
#include "GlobalValues.hpp"
#include "LevelEditor.hpp"
#include "InputController.hpp"
#include "SplashScreen.hpp"
#include "ScreenController.hpp"
#include "LogicController.hpp"
#include <iostream>
#include "DropDownMenu.hpp"
#include "ControlInfo.hpp"
#include "GameElementsExplanation.hpp"

std::mutex StateController::activeState_mutex;
StateController::States StateController::currentState;
StateController::States StateController::nextState;

State* StateController::createState(States state) {
	switch (state)
	{
	case StateController::MAIN_MENU:
		return new MainMenu();
	case StateController::LEVEL_FINISHED:
		return new EndLevel(false);
	case StateController::GAME_OVER:
		return new EndLevel(true);
	case StateController::GAME_EXPLANATION:
		return new GameExplanation();
	case StateController::HOME_BASE_MENU:
		return new HomeBase();
	case StateController::LEVEL_EDITOR:
		return new LevelEditor(1);
	case StateController::LOGIC_CONTROLLER:
		return new LogicController();
	case StateController::CONTROLS_INFO_SCREEN:
		return new ControlInfo();
	case StateController::GAME_ELEMENTS_INFO_SCREEN:
		return new GameElementsExplanation();
	default:
		return StateController::SPLASH_SCREEN; // If for some reason a faulty state was given
	}
}

State * StateController::getActiveState() {
	if (currentState != nextState) {
		//std::lock_guard<std::mutex> lock{ activeState_mutex }; // Active for scope
		// Remove from listeners
		activeState_mutex.lock(); // Claim mutex.
		if (activeState != nullptr) {
			InputController::getInstance().removeFunctionListener(activeState);
		}
		if (activeState != SPLASH_SCREEN) {
			delete activeState;
			activeState = nullptr;
		}
		activeState = createState(nextState);
		currentState = nextState;
		ScreenController::setActiveState(activeState);
		InputController::getInstance().addFunctionListener(activeState);
		activeState_mutex.unlock(); // Release mutex
	}
	return activeState;
}

void StateController::setActiveState(States newState){
	nextState = newState;
}

//Initialize splashscreen
State * StateController::SPLASH_SCREEN = new SplashScreen();

//Only one menu should be open at the time 
State * StateController::activeState = StateController::SPLASH_SCREEN; //init empty menu

																	  //drawing splashscreen default
																	  //set to false if you hate this splashScreen.
bool StateController::drawSplashScreen = true;

//load font one time only
sf::Font * State::m_font = nullptr;


int main() {

	ScreenController::initialize(); // initialize ScreenController


	ScreenController::getWindow()->setActive(false); // allow multithreaded rendering

	InputController::getInstance().setWindow(ScreenController::getWindow());

	while (ScreenController::getWindow()->isOpen()) {
		StateController::getActiveState()->update();

		sf::Event event;
		while (ScreenController::getWindow()->pollEvent(event)) {
			switch (event.type)
			{
				// Input events
			case sf::Event::EventType::MouseButtonPressed:
			case sf::Event::EventType::MouseButtonReleased:
			case sf::Event::EventType::KeyPressed:
			case sf::Event::EventType::KeyReleased:

				InputController::getInstance().update();

				break;

			case sf::Event::EventType::MouseMoved:
				StateController::getActiveState()->
					mouseHover(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				break;

				// Render events
			case sf::Event::EventType::Resized:
				ScreenController::changeResolution(event.size.width, event.size.height);
				break;

			case sf::Event::EventType::GainedFocus: // Focus gained
				ScreenController::updateFocus(true);
				break;

			case sf::Event::EventType::LostFocus: // Focus lost
				ScreenController::updateFocus(false);
				break;

			case sf::Event::EventType::Closed:
				ScreenController::shutDown();
				return 0;

			default:
				break;
			}
		}

		sf::sleep(sf::milliseconds(GlobalValues::UPDATE_SPEED_STATE));
	}
	return 0;
}

void StateController::lockActiveState(){
	StateController::activeState_mutex.lock();
}

void StateController::unlockActiveState(){
	StateController::activeState_mutex.unlock();
}