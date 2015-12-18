#pragma once

#include <SFML\Graphics.hpp>
#include "KeyMap.hpp"

/**
* InputListener interface to listen to new key presses
* @version 1.0
*/
class InputListener {
public:

	/**
	* When the key is pressed this function will be called.
	* @param func The functino key that was pressed.
	*/
	virtual void InputListener::keyPressed(KeyMap::function func) = 0;

	/**
	* Returns the keymap of all the keys the InputListener wants to listen to.
	* @return A list of KeyMap::function, so all the keys.
	*/
	virtual std::vector<KeyMap::function> InputListener::getKeyMap() = 0;

private:

	std::vector<KeyMap> theKeyMap;

};