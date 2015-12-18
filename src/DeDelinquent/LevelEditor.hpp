#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
#include "InputListener.hpp"
#include "State.hpp"
#include "DropDownMenu.hpp"
#include <mutex>
#include "PickupItems.hpp"
#include "Level.hpp"
#include "ExitPoint.hpp"
#include "GlobalValues.hpp"
#include "Guard.hpp"

/**
* Allows for the editing and creating a full Level with all the level elements such as Guards, Pickupitems, Exitpoints etc.
* @version 2
*/
class LevelEditor : public State
{

public:

	/**
	* Create a leveleditor with the start number of the first level you want to edit.
	* @param levelNumber The start level number, this number is used to define the first level LevelEditor will open.
	*/
	LevelEditor(int levelNumber); //starting number
	
	/**
	* Level editor is destructed and will remove all pointers and saved variables.                                                                     
	*/
	~LevelEditor();


	/**
	* KeyPressed implementation
	* @param func a KeyMap::function argument
	*/
	void keyPressed(KeyMap::function func) override;

	/**
	* Draws on a scrollable view
	* @param window the reference to the rendering window
	*/
	void drawScrollable(sf::RenderWindow & window) override;

	/**
	* Draws on a fixed view
	* @param window the reference to the rendering window
	*/
	void drawFixed(sf::RenderWindow & window) override;

	/**
	* Update implementation
	*/
	void update() override;

	/**
	* MouseHover implementation
	*/
	void mouseHover(sf::Vector2i position) override;

	/**
	* Returns all keys the LevelEditor will listen to.                                                                      
	*/
	std::vector<KeyMap::function> LevelEditor::getKeyMap() override;



private:

	struct textureObject{
		int layer = 0;
		sf::Sprite * objectSprite;
		bool collision;
		sf::Texture * objectTexture;
		std::string * textureFilename;
	};

	const std::string levelEditorTextureNames = std::string{ GlobalValues::MAIN_PATH() + "leveleditor/texture_list.json" };

	const std::string levelEditorPickupItems = std::string{ GlobalValues::MAIN_PATH() + "leveleditor/pickup_items.json" };

	std::shared_ptr<Level> theLevel = nullptr;

	std::shared_ptr<PickupItems> allItems = nullptr;

	std::shared_ptr<PickupItems::pickupItem> selectedPickupItem = nullptr;

	std::vector<std::shared_ptr<PickupItems::pickupItem>> specificItems;

	std::vector<std::shared_ptr<PickupItems::pickupItem>> placedPickupItems;

	std::shared_ptr<ExitPoint> exitPoints = nullptr;

	std::shared_ptr<Guard> currentAddingGuard;

	textureObject * currentSelectedSprite = nullptr;

	sf::Font * theFont;


	std::mutex mutex;
	
	DropDownMenu ddm = DropDownMenu(this);

	sf::Vector2f startPosButtons = sf::Vector2f{ 30, 30 };

	sf::Vector2f startPosPickupButtons;

	std::vector<textureObject> possibleTextureButtons;

	std::vector<KeyMap::function> allTheKeys = std::vector<KeyMap::function>{
			KeyMap::select,
			KeyMap::exit,
			KeyMap::moveUp,
			KeyMap::moveDown,
			KeyMap::moveLeft,
			KeyMap::moveRight,
			KeyMap::arrowUp,
			KeyMap::arrowDown,
			KeyMap::arrowLeft,
			KeyMap::arrowRight,
			KeyMap::use,
			KeyMap::middle
	};

	sf::Vector2f movementOffset;
	
	
	sf::Vector2f startPos;

	sf::Vector2f endPos;

	sf::CircleShape waypointCirclePatrouille;

	int clickedStartOrEnd;

	bool creatingGuardWaypoints = false;

	bool changeUsingArrowKeys = false;

	bool levelEditorLoaded = false;

	int selectedIndexSprite = -1;

	int loadedLevelNumber;

	std::vector<sf::RectangleShape> listOfRectangles;

	std::vector<sf::CircleShape> listOfCircles;

	std::vector<std::shared_ptr<Guard>> guards;

	std::vector<sf::Text> listOfTexts;


	void addRectangle(sf::Vector2f pos, sf::Color fill_color, sf::Vector2f size, int rotationAngle, int outline_thick = 0, sf::Color outline_color = { 0, 0, 0 });

	void addCircle(sf::Vector2f pos, sf::Color fill_color, int radius, int outline_thick = 0, sf::Color outline_color = { 0, 0, 0 });

	void addText(sf::Vector2f pos, sf::Color fill_color, std::string theText, int charSize, int rotation, sf::Text::Style style);

	void initializeAllItems(int levelNumber, bool removeLevel);

	void LevelEditor::initPickupItems();

	void selectLevelSpeficItems();

	void initLevelEditor();

	sf::CircleShape * LevelEditor::checkForCircleClick(sf::Vector2i mousePos);

	void LevelEditor::createPickupItem(bool mainitem, int expPoints, int levelNm, std::string & textureFilePath);

	void LevelEditor::createTextureButton(std::string textureFile, bool collision, int layer);

	textureObject * LevelEditor::checkForTextureButtonPress(sf::Vector2i mousePos);


	
};

