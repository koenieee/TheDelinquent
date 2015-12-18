#pragma once

#include <string>


/**
* GlobalValues is used to define some values used around the whole project
* @version 1.0
*/
class GlobalValues{
public:

	/**
	* Window width of main screen window
	*/
	static const unsigned int WINDOW_SIZE_X = 1024;

	/**
	* Window height of main screen window
	*/
	static const unsigned int WINDOW_SIZE_Y = 768;

	/**
	* Update speed of StateController; set of how fast the menus are drawed and how many times the inputController can listen to input events.
	*/
	static const unsigned int UPDATE_SPEED_STATE = 12; //value for while() state update speed

	/**
	* Used to determine the start location folder of the resource directory.                                                                      
	*/
	static const std::string MAIN_PATH() {
		return "../src/DeDelinquent/resources/";
	}

	/**
	* This bool can be set to true if you want to save the player data to the save file.                                                                  
	*/
	static const bool SAVE_PLAYER_DATA = true;
};

