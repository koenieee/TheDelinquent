#pragma once

#include "State.hpp"
#include "MainMenu.hpp"
#include "MainMenu.hpp"
#include "SplashScreen.hpp"
#include "LevelEditor.hpp"

/**
* Updates a State and provides methods to transition between states.
* @version 1.1
*/
class StateController{
public:

	/** 
	* Enumeration of all States that can be active.
	*/
	enum States
	{
		MAIN_MENU = 0x1,
		LEVEL_FINISHED,
		GAME_OVER,
		GAME_EXPLANATION,
		HOME_BASE_MENU,
		LEVEL_EDITOR,
		LOGIC_CONTROLLER,
		CONTROLS_INFO_SCREEN,
		GAME_ELEMENTS_INFO_SCREEN
	};

	/**
	* Creates a States pointer.
	*
	* @param state, a States enum.
	* @return a new @see State pointer.
	*/
	static State* StateController::createState(States state);

	/**
	* Sets the Active State.
	*
	* @param newState the @see State to change to.
	*/
	static void StateController::setActiveState(States newState);
	
	/**
	* Retrieves the currently active state
	* @return the active @see State
	*/
	static State * StateController::getActiveState();

	/**
	* Lock the Active State thread.
	*/
	static void StateController::lockActiveState();

	/**
	* Unlock the Active State thread.
	*/
	static void StateController::unlockActiveState();

	/**
	* This bool is used to determine if the splash screen is drawn behind the menu state.                                                                     
	*/
	static bool StateController::drawSplashScreen;

	/**
	* Static State connected to the Splash Screen.                                                                     
	*/
	static State * StateController::SPLASH_SCREEN;

private:
	/**
	* A pointer to the currently active menu
	*/
	static State * activeState;


	static States currentState;
	static States nextState;

	static std::mutex activeState_mutex;
};