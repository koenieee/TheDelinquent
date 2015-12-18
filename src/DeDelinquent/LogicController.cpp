#include <iostream>
#include "GlobalValues.hpp"
#include "AwarenessMeter.hpp"
#include "LogicController.hpp"
#include "StateController.hpp"
#include "ArtificialIntelligence.hpp"
#include "Guard.hpp"
#include "SoundBar.hpp"
#include "PickupItems.hpp"


LogicController::LogicController()
{

	loadLevelNumber(PlayerData::getInstance().getLevelNumber());

	
	Movable::setTheLevel(theLevel.get());

	updatebles.push_back(player1);

	drawables.push_back(theLevel);

	drawables.push_back(player1);

	ArtificialIntelligence::setPlayer(player1);
	ArtificialIntelligence::setLevel(theLevel);

	//Guard::saveAllGuards();
	std::vector<std::shared_ptr<Guard>> guards = Guard::loadGuards(theLevel->getLoadedLevelNumber(), false);

	for (auto g : guards){
		if (g->getLevelNumber() == theLevel->getLoadedLevelNumber()){
			updatebles.push_back(g);
			drawables.push_back(g);
			player1->addInteractable(g);
		}
	}

	player1->addInteractable(allPickupItems);
	drawables.push_back(allPickupItems);

	
	player1->addInteractable(exitPoints);
	
	drawables.push_back(allPickupItems);
	drawables.push_back(exitPoints);

	theFont = State::createFont();
	objectiveText.setPosition(sf::Vector2f{ (float)GlobalValues::WINDOW_SIZE_X / 9, (float)GlobalValues::WINDOW_SIZE_Y - 200 });
	objectiveText.setFont(*theFont);
	objectiveText.setCharacterSize(30);
	objectiveText.setString("-Vermijd bewakers\n-Zoek extra doelen\n-Vind main objective\n-Ga naar de uitgang");
	objectiveText.setStyle(sf::Text::Bold);
	objectiveText.setColor(sf::Color::Red);

	clock.restart();
	
}

LogicController::~LogicController(){
	ArtificialIntelligence::stopAllArtificialIntelligenceThreads();
	player1->clearInteractables();
	//clearing updatebales and drawables
	for (unsigned int i = 0; i< updatebles.size(); ++i) {
		if (updatebles[i] != nullptr) {
			//delete updatebles[i];
			updatebles[i] = nullptr;
		}
	}
	updatebles.clear();
	for (unsigned int i = 0; i < drawables.size(); ++i) {
		if (drawables[i] != nullptr) {
			//delete drawables[i];
			drawables[i] = nullptr;
		}
	}
	drawables.clear();
	ArtificialIntelligence::resetPlayerAndLevel();
	player1 = nullptr;
	theLevel = nullptr;
	
	exitPoints = nullptr;
	allPickupItems = nullptr;
	
}

void LogicController::loadLevelNumber(int levelNumber){
	currentLoadLevelNumber = levelNumber;

	if (theLevel.use_count() == 0) {
		theLevel = std::shared_ptr<Level>(new Level{ levelNumber });
	}
	if (exitPoints.use_count() == 0) {
		exitPoints = std::shared_ptr<ExitPoint>(new ExitPoint{ levelNumber, true });
	}
	if (allPickupItems.use_count() == 0) {
		allPickupItems = std::shared_ptr<PickupItems>(new PickupItems{ levelNumber, theLevel->getTotalLevelExperiencePoints() });
	}
	if (player1.use_count() == 0) {
		player1 = std::shared_ptr<Player>(new Player(theLevel->getPlayerStartPosition()));
	}
}

std::vector<KeyMap::function> LogicController::getKeyMap(){
	return LogicController::allTheKeys;
}

void LogicController::drawScrollable(sf::RenderWindow& window) {
	for (auto i : drawables) {
		i->draw(window);
	}
}

void LogicController::drawFixed(sf::RenderWindow& window){

	if (!(clock.getElapsedTime().asSeconds() >= LogicController::textSeconds))
	{
		sf::Color col = objectiveText.getColor();
		float elapsedTime = clock.getElapsedTime().asSeconds();
		col.a = elapsedTime == 0.0f ? 255 : 255 - 255 / (LogicController::textSeconds / elapsedTime);
		objectiveText.setColor(col);
		window.draw(objectiveText);
	}
	

	//TODO DO NOT UPDATE IN THIS DRAW FUNCTION


	SoundBar::getInstance().drawSoundBar(window);
	SoundBar::getInstance().updateSound();
	

	//drawing the awereness meter:

	//AwarenessMeter tryout
	//AwarenessMeter awarenessbleg;
	//static int i = 0;
	AwarenessMeter::getInstance().windowUpdate(window);
	//i = (i + 1) % 1024;

}

void LogicController::keyPressed(KeyMap::function func){
	switch (func){
		case KeyMap::exit:
			StateController::drawSplashScreen = true;
			StateController::setActiveState(StateController::MAIN_MENU);
			return;

		//case KeyMap::use:
		//	theLevel.saveLevelToFile();
		//break;
	}
	
	
}

void LogicController::mouseHover(sf::Vector2i position){

}

void LogicController::update()
{
	for (auto i : updatebles) {
		i->update();
	}
}