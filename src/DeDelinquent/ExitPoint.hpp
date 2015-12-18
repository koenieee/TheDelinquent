#pragma once
#include "UnpenetrableComponent.hpp"
#include <SFML/Graphics.hpp>
#include "IDrawableComponent.hpp"
#include "PlayerData.hpp"
#include "PickupItems.hpp"
#include "GlobalValues.hpp"


/**
* This class is responsible to load and save all ExitPoints in all the levels. 
* The player can see the exit points only when he grabbed the main objective in the level.
* @version 2.0
*/
class ExitPoint : public UnpenetrableComponent, public IDrawableComponent {
public:

	/**
	* ExitPoint can be constructed using a levelnumber and if the ExitPoint is constructed in the LevelEditor or just in normal game play.
	* @param currentLevelNumber The current level number is used to link the exit point to the level that is currently loaded.
	* @param playerMode Playermode can be set to true when the ExitPoints are dawned in the game play, and not in the LevelEditor.
	*/
	ExitPoint::ExitPoint(int currentLevelNumber = 0, bool playerMode = false);

	/**
	* This destructor will clear all pointers and objects that aren't used anymore.                                                                     
	*/
	ExitPoint::~ExitPoint();

	/**
	* Draw method that is overriden from IDrawableComponent.                                                                     
	*/
	void IDrawableComponent::draw(sf::RenderWindow & window) override;

	
	/**
	* Used to determine player collision, overidden from UnpenetrableComponenent.                                                                     
	*/
	bool UnpenetrableComponent::overlappingPosition(sf::FloatRect unpenetrable) override;

	/**
	* Remove the exitpoint connect to the levelnumber that was specified.
	* @param levelNumber Specify the levelnumber to remove all the exitpoints connected to that level.
	*/
	void ExitPoint::resetExitPoints(int levelNumber);

	/**
	* This will load all the saved exitpoints from a JSON file, located in a resource directory.
	*/
	void ExitPoint::loadExitsFromFile();

	/**
	* ALl the current exitpoints that are added using the addExitPoint function will be saved into a formatted JSON file.                                                                     
	*/
	void ExitPoint::saveExitsToFile();

	/**
	* Add another exitpoint to the specifed level number.
	* @param exitLocation The location of where the exit point is drawned and ofcrouce located.
	* @param levelNumber The levelnumber the exitpoint is connected to.
	*/
	void ExitPoint::addExitPoint(sf::Vector2f exitLocation, int levelNumber);


private:

	struct exitPointAndLevelnumber{
		sf::Sprite exitObject;
		int levelnumber = 0;
	};

	const std::string exitPointSaveLocationList = std::string{ GlobalValues::MAIN_PATH() + "levels/exitpoints_save_locations.json" };
	const std::string exitPointTextureLocation = std::string{ GlobalValues::MAIN_PATH() + "textures/exit.png" };

	std::vector<exitPointAndLevelnumber> exitPointsLocation;

	bool playerMode;

	sf::Texture exitTexture;
	sf::Sprite exitSprite;

	int currentLevelNumber;
};