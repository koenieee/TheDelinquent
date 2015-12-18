#pragma once

#include <SFML\Graphics.hpp>
#include "StateController.hpp"

/**
* Renders content on a seperate thread and allows for the modification of the scrollable view
* @version 1.0
*/
class ScreenController
{
public:

	/**
	* Initalizes the ScreenController and RenderWindow.
	* Afterwards a new Thread is started on which the rendering happens.
	*/
	static void ScreenController::initialize();
	/**
	* Shuts down the ScreenController and releases all resources used.
	* Also terminates the Thread it is running on.
	*/
	static void ScreenController::shutDown();

	/**
	* Returns an instance of the window
	*/
	static sf::RenderWindow* ScreenController::getWindow();

	/**
	* Change the resolution to the desired size
	* @param width the width of the window
	* @param height the height of the window
	*/
	static void ScreenController::changeResolution(unsigned int width, unsigned int height);

	/**
	* Updates the focus
	* This should only be called by the event handler
	* @param focus the current focus of the window
	*/
	static void ScreenController::updateFocus(bool focus);

	/**
	* Moves the scrollable view (offsets it) relative from its current position
	* @param move the offset
	*/
	static void ScreenController::moveView(sf::Vector2f move);

	/**
	* Centers the view to the specified coordinates
	* @param center the new center of the view
	*/
	static void ScreenController::centerView(sf::Vector2f center);

	/**
	* Retrieves the offset from the origin (0,0)
	* @return the offset
	*/
	static sf::Vector2f ScreenController::getMovementOffset();

	/**
	* Resets the scrollable view to its initial position (0,0)
	*/
	static void ScreenController::resetView();

	/**
	* This will change the current active state. 
	* @param activeState The active state you want to change to.
	*/
	static void ScreenController::setActiveState(State * activeState);



private:
	static void ScreenController::run();

	// Thread
	static sf::Thread* m_thread; // instance of (running) Thread
	static sf::RenderWindow* m_window; // instance of RenderWindow for rendering
	static bool m_running; // indicator for running thread

	// FPS
	static sf::Font* m_font;
	static sf::Clock m_fpsClock;
	static sf::Clock m_gameClock;
	static long m_nrDrawUpdates;
	static long m_avgFps;
	static unsigned int m_fpsLimit;

	// Graphics
	static unsigned int m_windowWidth;
	static unsigned int m_windowHeight;
	static bool m_focus;

	static sf::View m_view; // view for scrollable rendering
	static sf::View m_fixedView; // view for fixed rendering

	static sf::Vector2f movementOffset;
	static State * activeState;
};