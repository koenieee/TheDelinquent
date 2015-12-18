#include "splashScreen.hpp"
#include <iostream>
#include "GlobalValues.hpp"
#include "StateController.hpp"

SplashScreen::SplashScreen()
{
	State::loadBackground();

	clock.restart();
}

void SplashScreen::drawFixed(sf::RenderWindow& window){
	State::drawBackground(window);
}

void SplashScreen::update()
{
	if (clock.getElapsedTime().asSeconds() >= SplashScreen::seconds)
	{
		StateController::setActiveState(StateController::MAIN_MENU);
	}
}

void SplashScreen::keyPressed(KeyMap::function func){
	StateController::setActiveState(StateController::MAIN_MENU);
}

std::vector<KeyMap::function> SplashScreen::getKeyMap(){
	return SplashScreen::allTheKeys;
}