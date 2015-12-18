#pragma once

#include "UpdateableComponent.hpp"
#include "IDrawableComponent.hpp"
#include "DiskController.hpp"
#include "UnpenetrableComponent.hpp"



/**
* Level is used to hold information about the map of the level. Level can read from a JSON file and can also be saved to a JSON file
* Level can be designed using the LevelEditor.
* @version 2
*/
class Level: public IDrawableComponent, public UnpenetrableComponent{
public:

	/**
	* Level can be constructed using a Level name and if there will be a player or not.
	* @param levelName The name of the level 
	*/
	Level::Level(int levelNumber = 0);

	/**
	* Level can be destructed and will delete also all the used pointers.
	*/
	Level::~Level();


	/**
	* Used to draw all the level sprites and flexible components.
	*/
	void IDrawableComponent::draw(sf::RenderWindow & window) override;

	/*
	* Function to determine if a FloatRect is penetrating a level item.
	*/
	bool UnpenetrableComponent::overlappingPosition(sf::FloatRect unpenetrable) override;

	/**
	* Level can load itself from a JSON file, that is specified using the levelName
	*/
	void Level::loadLevelFromFile();

	/**
	* Level will save itself to the levelName specified by the constructor.
	*/
	void Level::saveLevelToFile();

	/**
	* Level can resort all sprites according to the layers specified by each mapObjectItem.                                                                     
	* So this will sort by layer, layer numbers that are high are last drawn, and lower numbers are first drawn.
	*/
	void Level::resortSprites(bool asc = true);

	/**
	* Add sprite to Level object, such as a wall or a floor etc. Mostly used in LevelEditor.
	*                                                                      
	* @param theSprite The sprite that will be added to this Level Object
	* @param layer Layer number to set 0 for lowest, higher for higher layers
	* @param texture The texture that must also be saved by Level, for the next time you load the level
	* @param textureFileName The filename of the texture so Level can next time create the texture itself
	*/
	void Level::addSprite(sf::Sprite & theSprite, int layer, bool collision, sf::Texture & texture, std::string textureFileName);

	/**
	* Function to change the layer of a existing sprite that is already added to Level.
	*                                                                      
	* @param indexKey The key that refers to the sprite you want to change
	* @param layer This can be set to something bigger than zero, or lower. But the layer number itself will only change 1 at the time.
	*/
	void Level::spriteToLayer(int indexKey, int layer);

	/**
	* Checks if a mouse position is currently inside a existing texture sprite from Level                                                                     
	* @param mousePos The mouse position you want to check
	* @return This will return the index key when the mouse was inside a existing sprite, if the mouse wasn't in a sprite this will return -1.
	*/
	int Level::checkSelectedSprite(sf::Vector2f mousePos);

	/**
	* Used to remove an existing item from Level                                                                      
	* @param indexKey The key from a sprite you want to remove, make sure it exists before running this function, or something will go wrong.
	*/
	void Level::removeSpriteAt(int indexKey);

	/**
	* This can change the position of an existing item (sprite) from Level                                                                     
	* @param indexKey The key of the sprite the position must be changed.
	* @param movePosition A Vector wich will define the .move() value of the selected sprite.
	* @return This function will return the new index key of the moved sprite, so when you want to move it again -> store that value ;)
	*/
	int Level::moveSpriteAt(int indexKey, sf::Vector2f movePosition);

	/**
	* A struct to create an easy reading of all the items Level contains                                                                     
	* layer is the layer number, zero is the lowest layer
	* objectSprite is the sprite connected to the layer and collision value, also containing a texture
	* collision is true of false to define if I can walk through your sprite object or not.
	*/
	struct mapObjectItem{
		int  layer = 0;
		sf::Sprite  objectSprite;
		bool  collision;
	};

	/**
	* It is possible to ask level what level is currently loaded                                                                      
	* @return The current loaded level number as Integer
	*/
	int Level::getLoadedLevelNumber();

	/**
	* This will remove the current loaded level file, so you can start again building the level.                                                                     
	*/
	void Level::removeCurrentLevelFile();

	/**
	* This will return a copy of all the Sprites that are currently in Level, if you want to change sprites use the appropriate functions written above.                                                                     
	* @return A vector containing all the information of all the Sprites. Including collision and layer values.
	*/
	std::vector<mapObjectItem> * Level::getAllSprites();

	/**
	* This function is used in the LevelEditor, you can specifiy the max experience the player can retrieve when finishing the level.                                                                     
	* @param totalExpPoints Set the total experience points player can get from finishing th level.
	*/
	void Level::setTotalLevelExperiencePoints(int totalExpPoints);

	/**
	* Returns the max experience the player can get when he finishes the level                                                                     
	* @return int value containing the max experience.
	*/
	int Level::getTotalLevelExperiencePoints();

	/**
	* Set the begin position of player                                                                     
	* @param plStartPos X and Y coordinates containing the start position of Player.
	*/
	void Level::setPlayerStartPosition(sf::Vector2f plStartPos);

	/**
	* Get the start position of Player                                                                     
	* @return sf::Vector2F containing the start position of player.
	*/
	sf::Vector2f Level::getPlayerStartPosition();

private:

	sf::Vector2f playerStartPosition = sf::Vector2f{ 300, 500 }; //default player position.

	int totalLevelExperiencePoints = 0;

	unsigned int currentLevelNumber = 0;

	//privately used to add a sprite from scratch (well not from scratch but from JSON)
	sf::Sprite Level::createSprite(sf::Texture & usingTexture, int posX, int posY, int height, int width, int rotation, double scale);

	std::string levelName;

	struct texureObjectItem{ //only used to store information about the textures, don't use or do something with this.
		bool collision;
		std::string * textureFileName;
		sf::Texture * theTexture;
	};

	//list of sprites generated by textures
	std::vector<mapObjectItem> allSprites; //please don't make this one public.

	//storing also textures, you will not need this. Only used to full save the level, including path names to textures etc.
	std::vector<texureObjectItem> allTextures;


};
