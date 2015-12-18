#pragma once

#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>

/**
* KeyMap contains all the keybindings and the enum functions
* @version 1.0
*/
class KeyMap{
public:

	/**
	fills the keyBoundList and mouseBoundList with functions
	*/
	KeyMap::KeyMap();

	const static enum function{
		changeLayerUP, //layer up in LevelEditor
		changeLayerDOWN, //layer down in LevelEditor
		removeObject,//remove object in LevelEditor
		saveLevel, //save level in LevelEditor
		reloadLevel,//reload level in LevelEditor
		changePosition,//change sprite position in LevelEditor
		nextLevel,//go to next level in LevelEditor
		prevLevel,//go to prev level  in LevelEditor
		removeLevel, //remove current level in LevelEditor
		resetPickupItems, //reset positions of all pickup items in LevelEditor
		setPlayerPosition, //set start position of player in LevelEditor
		setExpLvl, //set max experience points you can receive for LevelEditor
		middle, //press middle mouse button to switch between change position of sprite and map in LevelEditor
		addGuard, //used in leveleditor only
		resetLevelGuards,  //used in leveleditor only
		resetExitPoints, //reset the exit points leveleditor
		addExitPoint, //add another exit point leveleditor
		arrowUp,
		arrowDown,
		arrowLeft,
		arrowRight,
		moveUp, moveDown, moveLeft, moveRight,//moves the first player 
		pause,
		run, sneak,//lets the player run or sneak.
		use,//the use function for the player(example: open chest)
		exit,//return key, to navigate in windows and open ingame pause menu
		select,//an menu button works with this function
		options,//used to get more options (mostly in dropdown menu)
		setDropDownMenu,//used by dropdown menu.
		count//last enum to give the amount of functions back.DO NOT DELETE!
	};

	/**
	* Returns list of Keys
	*/
	sf::Keyboard::Key KeyMap::keyBoundList[KeyMap::count];
	
	/**
	* Returns list of MouseBounds
	*/
	sf::Mouse::Button KeyMap::mouseBoundList[KeyMap::count];

};
