#include "LevelEditor.hpp"
#include "StateController.hpp"
#include <iostream>
#include <fstream> 
#include "ScreenController.hpp"

// Iterator for texture buttons
static int itr;

LevelEditor::LevelEditor(int levelNumber)
{
	ScreenController::resetView();
	loadedLevelNumber = levelNumber;
	itr = 1;

	startPosPickupButtons = sf::Vector2f{ (float)GlobalValues::WINDOW_SIZE_X - 80, 12 }; //The pickups are at the right, so minus 80 to center them.
	
	exitPoints = std::shared_ptr<ExitPoint>(new ExitPoint{ loadedLevelNumber });

	theLevel = std::shared_ptr<Level>(new Level{ loadedLevelNumber });
	guards = Guard::loadGuards(loadedLevelNumber);

	rapidjson::Document * thed = DiskController::readFileIntoJson(levelEditorTextureNames);

	for (rapidjson::Value::MemberIterator m = (*thed).MemberBegin(); m != (*thed).MemberEnd(); ++m) { //reading texture settings located in a directory and a file.

		bool collision = m->value["COLLISION"].GetBool();
		std::string textureFileName = m->value["FILE"].GetString();
		int layer = m->value["LAYER"].GetInt();

		createTextureButton(textureFileName, collision, layer);

	}
	delete thed;
	thed = nullptr;

	initPickupItems();

	theFont = new sf::Font;
	theFont->loadFromFile(GlobalValues::MAIN_PATH() + "/fonts/arial.ttf");

	

	initLevelEditor();

	ddm.addToInputController();


}

void LevelEditor::initPickupItems(){
	allItems = nullptr;
	allItems = std::shared_ptr<PickupItems>(new PickupItems{ loadedLevelNumber });

	rapidjson::Document * pickups = DiskController::readFileIntoJson(levelEditorPickupItems);

	for (rapidjson::Value::MemberIterator m = (*pickups).MemberBegin(); m != (*pickups).MemberEnd(); ++m) { //reading pickups settings located in a directory and a file.
		bool mainitem = m->value["MAINITEM"].GetBool();
		std::string * textureFileName = new std::string{ m->value["FILE"].GetString() };
		allItems->addPickupItem(mainitem, loadedLevelNumber, *textureFileName);
	}
	pickups = nullptr;
	delete pickups;
}

void LevelEditor::initLevelEditor(){

	//creating toolbar, some magic values. Mostly found in my magic hat.
	addRectangle(sf::Vector2f{ 0, 0 }, sf::Color{ 255, 255, 204 }, sf::Vector2f{ 110, (float)GlobalValues::WINDOW_SIZE_Y }, 0, 3, sf::Color::Blue);
	//values found by trail and error
	addRectangle(sf::Vector2f{ (float)GlobalValues::WINDOW_SIZE_X - 110, 0 }, sf::Color{ 255, 255, 204 }, sf::Vector2f{ 110, (float)GlobalValues::WINDOW_SIZE_Y }, 0, 3, sf::Color::Blue);

	//values found by trail and error
	addText(sf::Vector2f{ 15, (float)GlobalValues::WINDOW_SIZE_Y - 20 }, sf::Color::Red, "Level: " + std::to_string(loadedLevelNumber), 15, 0, sf::Text::Italic);

	waypointCirclePatrouille.setPosition(sf::Vector2f{ 35, (float)GlobalValues::WINDOW_SIZE_Y - 40 }); //values found by trail and error
	waypointCirclePatrouille.setFillColor(sf::Color::Red);
	waypointCirclePatrouille.setRadius(10); 

	selectLevelSpeficItems();
}


LevelEditor::~LevelEditor(){
	std::cout << "~LevelEditor called!" << std::endl;
	ddm.removeFromInputController();
	if (allItems != nullptr) {
		allItems = nullptr;
	}
	if (currentSelectedSprite != nullptr) {
		delete currentSelectedSprite;
		currentSelectedSprite = nullptr;
	}
	for (unsigned int i = 0; i< placedPickupItems.size(); ++i) {
		if (placedPickupItems[i] != nullptr) {
			placedPickupItems[i] = nullptr;
		}
	}
	placedPickupItems.clear();
	for (auto & item : placedPickupItems) {
		if (item != nullptr) {
			if (item->pickupSprite != nullptr) {
				delete item->pickupSprite;
				item->pickupSprite = nullptr;
			}
			if (item->pickupTexture != nullptr) {
				delete item->pickupTexture;
				item->pickupTexture = nullptr;
			}
			if (item->pickupTextureFilename != nullptr) {
				delete item->pickupTextureFilename;
				item->pickupTextureFilename = nullptr;
			}
			item = nullptr;
		}
	}
	placedPickupItems.clear();
	if (selectedPickupItem != nullptr) {
		selectedPickupItem = nullptr;
	}
	for (auto & item : specificItems) {
		if (item != nullptr) {
			item = nullptr;
		}
	}
	specificItems.clear();
	for (auto & guard : guards) {
		if (guard != nullptr) {
			guard = nullptr;
		}
	}
	guards.clear();
	if (theLevel != nullptr) {
		theLevel = nullptr;
	}
}


void LevelEditor::selectLevelSpeficItems(){
	selectedPickupItem = nullptr;
	
	specificItems.clear();
	placedPickupItems.clear();
	specificItems = allItems->getLevelSpecificItems(loadedLevelNumber);
	
	int startNumber = 0;
	for (auto & currentPickup : specificItems){
		if (currentPickup->itemPlacedInLevel == false){
			if (startNumber == 0){
				currentPickup->pickupSprite->setPosition(sf::Vector2f{ (float)startPosPickupButtons.x, (float)startPosPickupButtons.y });
			}
			else{
				currentPickup->pickupSprite->setPosition(sf::Vector2f{ (float)startPosPickupButtons.x, (float)startNumber*(currentPickup->pickupTexture->getSize().y - 30) });
			}
			
			startNumber++;
		}
		else{
			placedPickupItems.push_back(currentPickup);
			
		}
	}

	for (auto & placedPickup : placedPickupItems){
		specificItems.erase(std::remove(specificItems.begin(), specificItems.end(), placedPickup), specificItems.end());
	}

}


void LevelEditor::createTextureButton(std::string textureFile, bool collision, int layer){
	sf::Texture & currentTexture = *new sf::Texture; //creating texture on the heap, otherwise it doesn't exists when I want it.


	if (!currentTexture.loadFromFile(textureFile))
	{
		throw std::exception("Couldn't load a texture");
	}
	currentTexture.setSmooth(true); //making smootie of texture
	currentTexture.setRepeated(true);

	sf::Sprite & sprite = *new sf::Sprite;
	sprite.setTexture(currentTexture);
	sprite.setPosition(startPosButtons.x, itr*(currentTexture.getSize().y)-40);
	sprite.setScale(sf::Vector2f{ (float)0.5, (float)0.5 }); //scale factor is 0,5 for all textures, pickups etc. Pickups and textures must be 100 x 100
	sprite.setRotation(0);


	LevelEditor::textureObject currentMapObjectItem;
	currentMapObjectItem.layer = layer;
	currentMapObjectItem.collision = collision;
	currentMapObjectItem.objectSprite = &sprite;
	currentMapObjectItem.objectTexture = &currentTexture;
	currentMapObjectItem.textureFilename = new std::string{ textureFile };

	possibleTextureButtons.push_back(currentMapObjectItem);
	itr++; //itr incremented for next item.
}

LevelEditor::textureObject * LevelEditor::checkForTextureButtonPress(sf::Vector2i mousePos){
	for (textureObject & sp : possibleTextureButtons){
		if (sp.objectSprite->getGlobalBounds().contains((sf::Vector2f)mousePos)){
			return &sp;
		}
	}
	return nullptr;
}

sf::CircleShape * LevelEditor::checkForCircleClick(sf::Vector2i mousePos){
	if (waypointCirclePatrouille.getGlobalBounds().contains((sf::Vector2f)mousePos)){
		return &waypointCirclePatrouille;
	}
	return nullptr;
}


std::vector<KeyMap::function> LevelEditor::getKeyMap(){
	if (levelEditorLoaded){
		levelEditorLoaded = false;
		ddm.removeFromInputController();
		
	} else {
		levelEditorLoaded = true;
		
	}
	return LevelEditor::allTheKeys;
}

void LevelEditor::initializeAllItems(int levelNumber, bool removeLevel){ //levelNumber can be 0= current level, -1 = one level back, +1 = one level up
	//resetting items;
	ScreenController::resetView();
	listOfCircles.clear();
	listOfTexts.clear();

	creatingGuardWaypoints = false;
	changeUsingArrowKeys = false;
	selectedIndexSprite = -1;
	clickedStartOrEnd = 0;
	selectedPickupItem = nullptr;
	delete currentSelectedSprite;


	startPos = sf::Vector2f{ 0, 0 };


	
	int newLevelNumber = 0;

	switch (levelNumber)
	{
	case -1: 
		if (theLevel->getLoadedLevelNumber() == 0){
			newLevelNumber = 0;
		}
		else{
			newLevelNumber = theLevel->getLoadedLevelNumber() + levelNumber;
		}
		break;
	case 0: newLevelNumber = theLevel->getLoadedLevelNumber();
		break;
	case 1: newLevelNumber = theLevel->getLoadedLevelNumber() + levelNumber;
		break;
	default:
		break;
	}
	
	loadedLevelNumber = newLevelNumber;
	


	theLevel = nullptr;

	theLevel = std::shared_ptr<Level>(new Level{ newLevelNumber });

	if (theLevel->getAllSprites()->empty() || removeLevel){ //when level is seen as empty, reset some older thingies:

		exitPoints->resetExitPoints(newLevelNumber);
		exitPoints->saveExitsToFile();

		allItems->resetPickupItems(newLevelNumber);
		allItems->savePickupsToFile();

		Guard::resetGuards(guards, newLevelNumber);
	

		if (removeLevel){
			theLevel->removeCurrentLevelFile();
			Guard::saveGuards(guards);

			theLevel = nullptr;

			theLevel = std::shared_ptr<Level>(new Level{ newLevelNumber });

		}
	}

	allItems = nullptr;
	exitPoints = nullptr;

	
	initPickupItems();

	exitPoints = std::shared_ptr<ExitPoint>(new ExitPoint{ newLevelNumber });

	
	guards = Guard::loadGuards(newLevelNumber);

	selectLevelSpeficItems();

	//add level Number text
	addText(sf::Vector2f{ 15, (float)GlobalValues::WINDOW_SIZE_Y - 60 }, sf::Color::Red, "Level: " + std::to_string(newLevelNumber), 15, 0, sf::Text::Italic);

	

}

void LevelEditor::keyPressed(KeyMap::function func){
	mutex.lock();
	sf::Vector2f dropDownMenuStartPosition = ddm.getDropDownMouseBeginPosition() - ScreenController::getMovementOffset();

	switch (func){


	case KeyMap::changeLayerUP: {
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		int foundedLevelSprite = theLevel->checkSelectedSprite(dropDownMenuStartPosition);
		if (foundedLevelSprite != -1){
			theLevel->spriteToLayer(foundedLevelSprite, 1);
		}
	}

	break;

	case KeyMap::changeLayerDOWN: {
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		int foundedLevelSprite = theLevel->checkSelectedSprite(dropDownMenuStartPosition);
		if (foundedLevelSprite != -1){
			theLevel->spriteToLayer(foundedLevelSprite, -1);
		}
	}
	break;

	case KeyMap::setExpLvl: {
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		int levelExpMax = 0;
		std::cout << "\n\n===================================\n";
		std::cout << "SET LEVEL EXPERIENCE: ";
		std::cin >> levelExpMax;
		std::cout << "\nYOU ENTERED: " << levelExpMax;
		std::cout << "\n===================================\n\n";
		theLevel->setTotalLevelExperiencePoints(levelExpMax);
	} 
	break;

	case KeyMap::setPlayerPosition: {
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		std::cout << "SETTING PLAYER POSITION TO: " << dropDownMenuStartPosition.x << ", " << dropDownMenuStartPosition.y;
		theLevel->setPlayerStartPosition(dropDownMenuStartPosition);
	}

	break;

	case KeyMap::addGuard: {
		creatingGuardWaypoints = false;
		listOfCircles.clear();
		std::cout << "ADDING GUARD AT POSITION: " << dropDownMenuStartPosition.x << ", " << dropDownMenuStartPosition.y;
		currentAddingGuard = std::shared_ptr<Guard>(new Guard{ std::vector < sf::Vector2f > {}, theLevel->getLoadedLevelNumber(), dropDownMenuStartPosition }); //init empty list
		guards.push_back(currentAddingGuard);
	}
	break;

	case KeyMap::addExitPoint: {
		creatingGuardWaypoints = false;
		listOfCircles.clear();
		std::cout << "ADDING EXITPOINT AT POSITION: " << dropDownMenuStartPosition.x << ", " << dropDownMenuStartPosition.y;
		exitPoints->addExitPoint(dropDownMenuStartPosition, theLevel->getLoadedLevelNumber());
	}
	break;

	case KeyMap::resetLevelGuards:{
		creatingGuardWaypoints = false;
		listOfCircles.clear();
		Guard::resetGuards(guards, theLevel->getLoadedLevelNumber());
		Guard::saveGuards(guards);
	}
    break;

	case KeyMap::resetExitPoints:{
		creatingGuardWaypoints = false;
		listOfCircles.clear();
		exitPoints->resetExitPoints(theLevel->getLoadedLevelNumber());
	}
	break;

	case KeyMap::resetPickupItems:{
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		ScreenController::resetView();
		allItems->resetPickupItems(theLevel->getLoadedLevelNumber());
		allItems->savePickupsToFile();
		initPickupItems();
		selectLevelSpeficItems();
	}
	break;

	case KeyMap::moveUp: {
		if (changeUsingArrowKeys && selectedIndexSprite != -1){ //selecting sprite item, so now we change the position of the item instead of the whole view.

			selectedIndexSprite = theLevel->moveSpriteAt(selectedIndexSprite, sf::Vector2f{ 0, -1 });
		}
		else{
			ScreenController::moveView({ 0, -20 }); //move view every time by 20 pixels.
		}
	}
	break;

	case KeyMap::middle: { //change position back to view.
		changeUsingArrowKeys = false;
		selectedIndexSprite = -1;
	}
	break;

	case KeyMap::moveDown: {
		if (changeUsingArrowKeys && selectedIndexSprite != -1){

			selectedIndexSprite = theLevel->moveSpriteAt(selectedIndexSprite, sf::Vector2f{ 0, 1 });
		}
		else{
			ScreenController::moveView({ 0, 20 }); //move view every time by 20 pixels.
		}
		
	}
	break;

	case KeyMap::moveLeft: {
		if (changeUsingArrowKeys && selectedIndexSprite != -1){ 

			selectedIndexSprite = theLevel->moveSpriteAt(selectedIndexSprite, sf::Vector2f{ -1, 0 });
		}
		else{

			ScreenController::moveView({ -20, 0 }); //move view every time by 20 pixels.

		}
	}
	break;

	case KeyMap::moveRight: {
		if (changeUsingArrowKeys && selectedIndexSprite != -1){
			
			selectedIndexSprite = theLevel->moveSpriteAt(selectedIndexSprite, sf::Vector2f{ 1, 0 });
		}
		else{

			ScreenController::moveView({ 20, 0 });

		}
	}
	break;


	case KeyMap::changePosition: {
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		std::cout << "\nUSE WASD KEYS TO MOVE THE SELECTED SPRITE, PRESS MIDDLE MOUSE BUTTON TO START MOVING LEVEL AGAIN\n";
		changeUsingArrowKeys = true;
		selectedIndexSprite = theLevel->checkSelectedSprite(dropDownMenuStartPosition);

	}
	break;

	case KeyMap::removeObject: {
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		int foundedLevelSprite = theLevel->checkSelectedSprite(dropDownMenuStartPosition);
		if (foundedLevelSprite != -1){
			theLevel->removeSpriteAt(foundedLevelSprite);
			
		}
	}
	break;

	case KeyMap::saveLevel:
	{
		listOfCircles.clear();
		creatingGuardWaypoints = false;
		theLevel->saveLevelToFile();
		Guard::saveGuards(guards);
		allItems->savePickupsToFile();
		exitPoints->saveExitsToFile(); 
	}
	break;

	case KeyMap::reloadLevel:
	{
		initializeAllItems(0, false);

	}
	break;

	case KeyMap::nextLevel:
	{
		initializeAllItems(1, false);

	
	}
	break;

	case KeyMap::prevLevel:
	{
		initializeAllItems(-1, false);

	}
	break;

	case KeyMap::removeLevel:
	{

		initializeAllItems(0, true); //deleteing current level

	}
	break;

	case KeyMap::exit:
	{
		mutex.unlock(); // Unlock to prevent mutex destruction while locked
		StateController::drawSplashScreen = true;
		StateController::setActiveState(StateController::MAIN_MENU);

		return;
	}

	case KeyMap::select: 
	{
		sf::Vector2i currentMousePos = InputController::getInstance().getMousePos();

		sf::CircleShape * selectedCircle = checkForCircleClick(currentMousePos);

		if (allItems->checkForPickUpCheck(currentMousePos) != nullptr){
			selectedPickupItem = allItems->checkForPickUpCheck(currentMousePos);
		}

		else if (selectedPickupItem != nullptr){
			if (selectedPickupItem->pickupSprite != nullptr){
				selectedPickupItem->pickupSprite->setPosition((sf::Vector2f)currentMousePos - ScreenController::getMovementOffset());
			}
			
			selectedPickupItem->itemPlacedInLevel = true;
			specificItems.erase(std::remove(specificItems.begin(), specificItems.end(), selectedPickupItem), specificItems.end());

			placedPickupItems.push_back(selectedPickupItem);
			selectedPickupItem = nullptr;
		}


		else if (checkForTextureButtonPress(currentMousePos) != nullptr){ //Doing something when you select a texture item from the toolbar left.
			if (currentSelectedSprite != nullptr){ //cleaning older texture.
				delete currentSelectedSprite;
				currentSelectedSprite = nullptr;
			}
			currentSelectedSprite = new textureObject;
			currentSelectedSprite->collision = checkForTextureButtonPress(currentMousePos)->collision;
			currentSelectedSprite->layer = checkForTextureButtonPress(currentMousePos)->layer;
			currentSelectedSprite->objectSprite = new sf::Sprite{ *checkForTextureButtonPress(currentMousePos)->objectSprite };
			currentSelectedSprite->objectTexture = new sf::Texture{ *checkForTextureButtonPress(currentMousePos)->objectTexture };
			currentSelectedSprite->textureFilename = checkForTextureButtonPress(currentMousePos)->textureFilename;

			addRectangle(
				sf::Vector2f{ 
				currentSelectedSprite->objectSprite->getPosition().x - 2, //outside line of sprite.
				currentSelectedSprite->objectSprite->getPosition().y - 2
				}, 
				sf::Color::Red, 
				sf::Vector2f{ 
					(((float)currentSelectedSprite->objectSprite->getTextureRect().width)*currentSelectedSprite->objectSprite->getScale().x) + 4,
					(((float)currentSelectedSprite->objectSprite->getTextureRect().height)*currentSelectedSprite->objectSprite->getScale().y) + 4
				},
				0 //no outline thickness, because thsi rectangle is the outline tickies.
			);
		}
		else if (currentSelectedSprite != nullptr){ //this means you selected a texture item.
			if (clickedStartOrEnd == 0){ //first time you click, and you also selected a texture item.
				movementOffset = ScreenController::getMovementOffset();//ScreenController::getWindow()->getDefaultView(). - ScreenController::getWindow()->getView().getViewport()
				startPos = (sf::Vector2f)currentMousePos - movementOffset; //- bla;
				currentSelectedSprite->objectSprite->setPosition(startPos);
				currentSelectedSprite->objectSprite->setScale(sf::Vector2f{ 1, 1 });
				currentSelectedSprite->objectSprite->setTextureRect(sf::IntRect(0, 0, 0, 0)); //sprite is first not visible as long you didn't change mouse pos
				clickedStartOrEnd = 1;
			}
			else if (clickedStartOrEnd == 1){//second step and final to create a new sprite and connected to Level.
				endPos = (sf::Vector2f)currentMousePos - movementOffset;

				
				currentSelectedSprite->objectSprite->setTextureRect(sf::IntRect(0, 0, endPos.x - startPos.x, endPos.y - startPos.y));

				theLevel->addSprite(*currentSelectedSprite->objectSprite, currentSelectedSprite->layer, currentSelectedSprite->collision, *currentSelectedSprite->objectTexture, *currentSelectedSprite->textureFilename);
				
				//resetting back to some default values, otherwise this will interfere with other functions (e.g. dropdownmenu)
				clickedStartOrEnd = 0;
				startPos = sf::Vector2f{ 0, 0 };
				delete currentSelectedSprite;
				currentSelectedSprite = nullptr;
				listOfRectangles.erase(listOfRectangles.end() - 1);

			}
		}
		
		else if (selectedCircle != nullptr){ //placing guard waypoints
			if (creatingGuardWaypoints){
				creatingGuardWaypoints = false; //when clicking the waypoints again, you can stop creating waypoints for the guards.
			}
			else{
				creatingGuardWaypoints = true;
			}
		}
		else if (creatingGuardWaypoints){
			if (currentAddingGuard != nullptr){
				sf::Vector2f placementLocation = (sf::Vector2f)currentMousePos - ScreenController::getMovementOffset();
				addCircle(placementLocation, sf::Color::Red, 10); //red dot circle as waypoint
				currentAddingGuard->addPatrolWaypoint(placementLocation);
			}
		}
	}

	break;
	
	case KeyMap::setDropDownMenu:
		
			ddm.addFunctionList(std::vector < std::pair<KeyMap::function, std::string> > { //adding functions and items to dropdownmenu
				{KeyMap::changeLayerUP, "Change Layer UP"},
				{KeyMap::changeLayerDOWN, "Change Layer DOWN" },
				{KeyMap::changePosition, "Change Position" },
				{KeyMap::removeObject, "Remove Object" },
				
				{KeyMap::setPlayerPosition, "Set Player Position" },
				{KeyMap::addGuard, "Add Guard" },
				{KeyMap::addExitPoint, "Add Exit Point" },
				
				{KeyMap::resetPickupItems, "Reset Pickups" },
				{KeyMap::resetLevelGuards, "Reset Guards" },
				{KeyMap::resetExitPoints, "Reset Exit Points" },
				{KeyMap::setExpLvl, "Set Experience Level" },
				{KeyMap::reloadLevel, "Reload Level" },
				{KeyMap::nextLevel, "Next Level" },
				{KeyMap::removeLevel, "REMOVE LEVEL" },
				{KeyMap::prevLevel, "Previous Level" },
				{KeyMap::saveLevel, "Save Level" }
				
			});
		

	break;

	}
	mutex.unlock();

}

void LevelEditor::mouseHover(sf::Vector2i position){
	if (startPos != sf::Vector2f{ 0, 0 } && currentSelectedSprite != nullptr){
		mutex.lock();
		sf::Vector2f currentMousPos = (sf::Vector2f)position - movementOffset;
		

		currentSelectedSprite->objectSprite->setTextureRect(
			sf::IntRect(0, 0, 
			(currentMousPos.x - startPos.x), 
			(currentMousPos.y - startPos.y)) 
		);
		mutex.unlock();
	}
	
}

void LevelEditor::update(){

}


void LevelEditor::addRectangle(sf::Vector2f pos, sf::Color fill_color, 
	sf::Vector2f size, int rotationAngle, int outline_thick, sf::Color outline_color){
	sf::RectangleShape trd;
	trd.setPosition(pos);
	trd.setFillColor(fill_color);
	trd.setSize(size);
	trd.setRotation((float)rotationAngle);
	trd.setOutlineThickness((float)outline_thick);
	trd.setOutlineColor(outline_color);

	listOfRectangles.push_back(trd);
}

void LevelEditor::addCircle(sf::Vector2f pos, sf::Color fill_color, int radius, 
	int outline_thick, sf::Color outline_color){
	sf::CircleShape cs;
	cs.setPosition(pos);
	cs.setFillColor(fill_color);
	cs.setRadius((float)radius);
	cs.setOutlineThickness((float)outline_thick);
	cs.setOutlineColor(outline_color);

	listOfCircles.push_back(cs);
}

void LevelEditor::addText(sf::Vector2f pos, sf::Color fill_color, std::string theText,
	int charSize, int rotation, sf::Text::Style style){
	sf::Text tt;
	tt.setPosition(pos);

	tt.setFont(*theFont);
	tt.setColor(fill_color);
	tt.setString(theText);
	tt.setCharacterSize(charSize);
	tt.setRotation((float)rotation);
	tt.setStyle(style);

	listOfTexts.push_back(tt);

}

void LevelEditor::drawFixed(sf::RenderWindow & theWindow) {
	mutex.lock();
	for (sf::RectangleShape tt : listOfRectangles){
		theWindow.draw(tt);
	}
	for (sf::Text tt : listOfTexts){
		theWindow.draw(tt);
	}
	theWindow.draw(waypointCirclePatrouille);

	//drawing LevelEditor Texture Buttons
	for (textureObject sp : possibleTextureButtons){
		theWindow.draw(*sp.objectSprite);
	}

	for (auto & pp : specificItems){ //not yet placed pickup items. So these are displayed at the right
		theWindow.draw(*pp->pickupSprite);
	}

	
	ddm.draw(theWindow);
	mutex.unlock();
}

void LevelEditor::drawScrollable(sf::RenderWindow & theWindow) {
	

	mutex.lock();

	if (theLevel != nullptr){
		theLevel->draw(theWindow);

		if (currentSelectedSprite != nullptr && startPos != sf::Vector2f{ 0, 0 }){
			theWindow.draw(*currentSelectedSprite->objectSprite);
		}

		for (auto & drawableGuard : guards){
			if (drawableGuard != nullptr){
				if (drawableGuard->getLevelNumber() == theLevel->getLoadedLevelNumber()){
					drawableGuard->draw(theWindow);
				}
			}
		}

		exitPoints->draw(theWindow);

		for (auto & pp : placedPickupItems){
			if (pp->pickupSprite != nullptr){
				theWindow.draw(*pp->pickupSprite);
			}
			
		}

		for (sf::CircleShape tt : listOfCircles){
			theWindow.draw(tt);
		}
	}

	mutex.unlock();

	
	
}



