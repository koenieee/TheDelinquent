#pragma once
#include <string>
#include "UnpenetrableComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "DiskController.hpp"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "IDrawableComponent.hpp"
#include "PlayerData.hpp"
#include "GlobalValues.hpp"
#include <mutex>
#include <memory>

/**
* Keeps track of all possible pickup items in all level. This will also save the location and settings of the different pickups.
* @version 1.0
*/
class PickupItems : public UnpenetrableComponent, public IDrawableComponent {
public:

	/**
	* PickupItems can be initialized using a currentLevelNumber and the Total Experience you can get for finishing the level.                                                                     
	* @param currentLevelNumber Set the current loaded level number.
	* @param totalExperienceLevelPoints Sets the total experience according to the level number.
	*/
	PickupItems::PickupItems(int currentLevelNumber = 0, int totalExperienceLevelPoints = 300);

	/**
	* This destructor will remove all items and pointers that were used to process PickupItems.                                                                     
	*/
	PickupItems::~PickupItems();

	/**
	* This function will add a pickup item.                                                                     
	* @param isMainObject This bool can be set to specify if the pickup item is the Level Main Objective
	* @param levelNumber This integer can be set to specify which pickup belongs to a level
	* @param textFileName The texture filename of the pickup item.
	* @param findLocation The location where the pickup item can be found.
	* @param playerPickupItem This bool specifies if the player has already found the item or not.
	* @param itemPlacedInLevel This bool specifies if the item is placed in the level.
	*/
	void PickupItems::addPickupItem(bool isMainObject, int levelNumber, std::string textFileName, sf::Vector2f findLocation = sf::Vector2f{ 0, 0 }, bool playerPickupedItem = false, bool itemPlacedInLevel = false);

	/**
	* This function checks if a Movable object is moving against or over a pickup item.                                                                     
	* @param unpenetrable The movable item you want to check the pickup to.
	* @return True of False if the Movable object was in a radius of the pickup item.
	*/
	bool UnpenetrableComponent::overlappingPosition(sf::FloatRect unpenetrable);

	/**
	* This function will reset all the find locations of the pickup items according to a specified level number.                                                                     
	* @param levelNumber The level number of the items you want to reset.
	*/
	void PickupItems::resetPickupItems(int levelNumber);

	/**
	* Struct containing all the information for the Pickup Items.                                                                     
	*
	*/
	struct pickupItem{
		sf::Texture * pickupTexture; //the pickup texture 
		sf::Sprite * pickupSprite; //the pickup sprite, created from a texture
		std::string * pickupTextureFilename; //the filename of the loaded texture
		int levelNumber = 0; //the level number where the pickup to belongs.
		bool mainObjectItem = false; //a bool to check if it was a main objective or not
		bool playerPickupedItem = false; //a bool to check if the player pickup the item already or not.
		bool itemPlacedInLevel = false; //a bool to check if the item is already placed in the level, used in levelEditor
	};

	/**
	* Used to calculate the experience the player when the player pickup the Main Objective item.                                                                     
	* @return The experience the player will receive when finishing the level.
	*/
	int PickupItems::calculateExperiencePoints();

	/**
	* This function is used in LevelEditor to check if the mouse is inside a pickup item so the item can be placed somewhere on the map.                                                                     
	* @param mousePos The mouse location
	* @return The pickupItem pointer when the mouse was somewhere in between the localbounds. This will return a nullpointer if not.
	*/
	std::shared_ptr<pickupItem> PickupItems::checkForPickUpCheck(sf::Vector2i mousePos);

	/**
	* This will return a list of all the PickupItems you can get according to  a specified level number                                                                     
	* @param levelNumber The levelnumber to get the items for.
	* @return a List of pickupItem containing all the information needed.
	*/
	std::vector<std::shared_ptr<pickupItem>> PickupItems::getLevelSpecificItems(int levelNumber);

	/**
	* Used to save all the pickups items from all levels to one pickup file.                                                                     
	*/
	void PickupItems::savePickupsToFile();

	/**
	* Used to load all the pickup items for all levels.
	*/
	void PickupItems::loadPickupsFromFile();

	/**
	* Function to draw the pickup items on the Level.                                                                     
	*
	*/
	void IDrawableComponent::draw(sf::RenderWindow & window) override;


private:
	const std::string pickupItemSaveLocationList = std::string{ GlobalValues::MAIN_PATH() + "levels/pickup_save_location.json" };

	int currentLevelNumber;
	int maxLevelExperience;

	std::vector<std::shared_ptr<pickupItem>> levelSpecificPickups;

	sf::Text notifcationText;

	sf::Vector2f notifTextLocation;

	std::shared_ptr<sf::Font> theFont = nullptr;

	sf::Clock clock;
	int textSeconds = 4;
	bool renderNotification = false;

	std::string playerFoundItem = std::string{"Item Gevonden"};
	std::string playerFoundMainItem = std::string{ "Zoek Uitgang" };

	std::mutex mutex;

	std::vector<std::shared_ptr<pickupItem>> allPickupItems;

};

