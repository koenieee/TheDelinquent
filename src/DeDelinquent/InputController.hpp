#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include "InputListener.hpp"
#include "KeyMap.hpp"
#include <mutex>
#include <list>

/**
* InputController
* @version 2.1
*/
class InputController {

public:

	/**
	* Uses the singleton pattern to create a new instance of the InputController
	* @return A pointer to the InputController
	*/
	static InputController& InputController::getInstance(){
		static InputController instance;
		return instance;
	}

	/**
	* Sets the window for the InputController, this is necessary for the relative position for the mouse and the events from the window.
	* @param gameWindow will be set as window for the InputController
	*/
	void InputController::setWindow(sf::Window* gameWindow);

	/**
	* Check if the key that is bound to the given function is pressed
	* @param func the function that will be checked
	* @return true if the button is pressed
	*/
	bool InputController::getfunction(KeyMap::function func);

	/**
	* Gives the mouse position relative to the window
	* @return the mouse position relative to the window as an sf:Vector2i
	*/
	sf::Vector2i InputController::getMousePos();

	/**
	* Adds an InputListener pointer with the function to an listener list
	* @param func the function the check when there is an event before calling the InputListener
	* @param listener the InputListener that will be called when all conditions are met.
	*/
	void InputController::addFunctionListener(InputListener * listener);

	/**
	* Removes the InputListener for the given button from the listener list
	* @param func only removes an listener if it equals the function from the list
	* @param listener only removes an listener if it equals the listener from the list
	*/
	void InputController::removeFunctionListener(InputListener* listener);

	/**
	Checks if buttons are pressed or released, and give the inputliseners an update if the key is pressed (not when released)
	*/
	void InputController::update();

	/**
	* Allow only one instance, so no copyable.                                                                     
	*/
	InputController(InputController const&) = delete;

private:
	bool changingMap = false;
	
	struct functionAndListener{
		std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>();
		InputListener* listener = nullptr;
	};

	struct keyIsPressed{
		KeyMap::function pressedKey;
		InputListener* listener;
	};


	InputController();

	std::list<functionAndListener> functionEvent;
	std::list<keyIsPressed> saveEvent;

	sf::Window* window;

	KeyMap KeyMap;


};