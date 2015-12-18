#include "Level.hpp"	
#include "DiskController.hpp"
#include "rapidjson\document.h"
#include <iostream>
#include <fstream>
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include <string>
#include <string.h>
#include "ArtificialIntelligence.hpp"
#include "PathFinding.hpp"
#include "GlobalValues.hpp"
#include "Guard.hpp"

Level::Level(int levelNumber)
{
	if (levelNumber < 1){ //no minus one level
		levelNumber = 1;
	}
	currentLevelNumber = levelNumber;
	levelName = GlobalValues::MAIN_PATH() + "levels/level_map_" + std::to_string(levelNumber) + ".json";


	loadLevelFromFile(); //loading level from file using specified filename above.
}


int Level::getLoadedLevelNumber(){
	return currentLevelNumber;
}

void Level::setTotalLevelExperiencePoints(int totalExpPoints){
	totalLevelExperiencePoints = totalExpPoints;
}

int Level::getTotalLevelExperiencePoints(){
	return totalLevelExperiencePoints;
}

void Level::setPlayerStartPosition(sf::Vector2f plStartPos){
	playerStartPosition = plStartPos;
}

sf::Vector2f Level::getPlayerStartPosition(){
	return playerStartPosition;
}

int Level::checkSelectedSprite(sf::Vector2f mousePos){
	resortSprites(false);
	int iterator = 0;
	for (mapObjectItem currentObject : allSprites){
		if (currentObject.objectSprite.getGlobalBounds().contains(mousePos)){
			return iterator; //return index key of sprite located under mouse cursor
			resortSprites(true);
		}
		iterator++;
	}
	resortSprites(true);
	return -1; //no mouse under your nose? Return -1
}

void Level::removeSpriteAt(int indexKey){
	allSprites.erase(allSprites.begin() + indexKey); //remove sprite from sprite list, when indexKey is outside or -1, this will throw an error.
}

void Level::removeCurrentLevelFile(){
	std::remove(levelName.c_str());
}

int Level::moveSpriteAt(int indexKey, sf::Vector2f movePosition){

	mapObjectItem foundedItem = allSprites.at(indexKey);

	foundedItem.objectSprite.move(movePosition);

	removeSpriteAt(indexKey); //first removing original sprite.
	allSprites.push_back(foundedItem); //adding changed sprite
	return allSprites.size() - 1; //returning new index key, ofcourse this is just the last one. Because we just added it ;-)

}

void Level::spriteToLayer(int indexKey, int layerUpOrDown){
	
	mapObjectItem foundedItem = allSprites.at(indexKey);
	if (layerUpOrDown > 0){
		foundedItem.layer += 1;
	}
	else{
		foundedItem.layer -= 1;
	}
	removeSpriteAt(indexKey); //first removing sprite
	allSprites.push_back(foundedItem); //now adding it.
	
	resortSprites(); //sort everything again by layer. So we can draw the correct order.
}

void Level::resortSprites(bool asc){

	std::sort(allSprites.begin(), allSprites.end(),
		[&](const mapObjectItem& lhs, const mapObjectItem& rhs)
		{
			if (asc){
				return lhs.layer < rhs.layer; //sorting asc, to get the 
			}
			else{
				return lhs.layer > rhs.layer;
			}
			
		}
	);
}

std::vector<Level::mapObjectItem> * Level::getAllSprites(){
	return &allSprites; //returning copy of allSprites. Do not use, even if you really need it.
}


Level::~Level(){
	//removing allsprites
	allSprites.clear();

	for (texureObjectItem ctObj : allTextures){
		delete (ctObj.textureFileName);
		delete (ctObj.theTexture);
	}
	allTextures.clear();

}


void Level::loadLevelFromFile(){

	std::ifstream infile(levelName);
	if (!infile.good()){ //if file doesn't exists create empty level, only containting {}, otherwise rapidjson will fail.
		std::ofstream out(levelName);
		out << "{\n\n}";
		out.close();
	}
	else{

		rapidjson::Document * levelDoc = DiskController::readFileIntoJson(levelName);

		if ((*levelDoc).HasMember("TOTAL_EXPERIENCE")){ //checking if the file isn't empty.

			int lvlPoints = (*levelDoc)["TOTAL_EXPERIENCE"].GetInt();

			setTotalLevelExperiencePoints(lvlPoints);

			int playerX = (*levelDoc)["PLAYER_START_X"].GetInt();
			int playerY = (*levelDoc)["PLAYER_START_Y"].GetInt();

			playerStartPosition = sf::Vector2f{ (float)playerX, (float)playerY };

			for (rapidjson::Value::MemberIterator m = (*levelDoc)["TEXTURES"].MemberBegin(); m != (*levelDoc)["TEXTURES"].MemberEnd(); ++m) {

				bool collision = m->value["COLLISION"].GetBool(); //check if texture is collision based

				sf::Texture * currentTexture = new sf::Texture; //these pointers will exists until level is destructed
				std::string * textureFileName = new std::string; //these pointers will exists until level is destructed

				*textureFileName = m->value["TEXTURE_FILE"].GetString(); //creating std string pointer to store the textureFilename, this must be a pointer, or it won't work.

				if (!currentTexture->loadFromFile(*textureFileName))
				{
					throw std::exception("Couldn't load a texture");
				}
				currentTexture->setSmooth(true); //making smootie of texture
				currentTexture->setRepeated(true); //possible to let texture repeate itself

				texureObjectItem currentTextureObject;
				currentTextureObject.collision = collision;
				currentTextureObject.textureFileName = textureFileName;
				currentTextureObject.theTexture = currentTexture;

				allTextures.push_back(currentTextureObject); //adding creating texture to texture list, so we can also write it back to JSON when the time is right


				if (m->value["LIST"].IsObject()) { //reading subobject, so all the textureitems.

					for (rapidjson::Value::MemberIterator nm = m->value["LIST"].MemberBegin(); nm != m->value["LIST"].MemberEnd(); ++nm) { //looping through every texture item.

						if (nm->value.IsObject()) {

							const rapidjson::Value& subsub = nm->value;
							int layer = subsub["layer"].GetInt();
							int posX = subsub["posX"].GetInt(); //startpoint
							int posY = subsub["posY"].GetInt(); //startpoint of the sprite Y
							int height = subsub["height"].GetInt();
							int width = subsub["width"].GetInt();
							int rotation = subsub["rotation"].GetInt();
							double scale = subsub["scale"].GetDouble();

							mapObjectItem currentMapObjectItem; //saving item in a struct, to access it later easily.
							currentMapObjectItem.layer = layer;
							currentMapObjectItem.collision = collision;
							currentMapObjectItem.objectSprite = createSprite(*currentTexture, posX, posY, height, width, rotation, scale);

							allSprites.push_back(currentMapObjectItem); //adding sprite we creating to all the sprites.

						}
					}
				}
			}
	
			//cleaning up one pointer.
			delete levelDoc;

			//sorting by layer. So we can fake the layer system.
			resortSprites();
		}
	}
}




void Level::saveLevelToFile(){

	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();
	
	rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();


	int texture_iterator = 0; //giving number to different textures.

	rapidjson::Value valLevelExperiencePoints(rapidjson::kNumberType);
	valLevelExperiencePoints.SetInt(totalLevelExperiencePoints); 
	jsonDoc.AddMember("TOTAL_EXPERIENCE", valLevelExperiencePoints, jsonDoc.GetAllocator()); 

	rapidjson::Value playerStartX(rapidjson::kNumberType);
	playerStartX.SetInt((int)playerStartPosition.x);
	jsonDoc.AddMember("PLAYER_START_X", playerStartX, jsonDoc.GetAllocator());

	rapidjson::Value playerStartY(rapidjson::kNumberType);
	playerStartY.SetInt((int)playerStartPosition.y); //setting collision true, or false. That's the question.
	jsonDoc.AddMember("PLAYER_START_Y", playerStartY, jsonDoc.GetAllocator()); //adding COLLISION member to json object

	rapidjson::Value allJSONTextures(rapidjson::kObjectType); //creating one head item for one texture including the specs
	allJSONTextures.SetObject();

	

	for (texureObjectItem currentTexture : allTextures){

		rapidjson::Value MainObject(rapidjson::kObjectType); //creating one head item for one texture including the specs
		MainObject.SetObject();


		rapidjson::Value valCollisionObject(rapidjson::kFalseType);
		valCollisionObject.SetBool(currentTexture.collision); //setting collision true, or false. That's the question.
		MainObject.AddMember("COLLISION", valCollisionObject, jsonDoc.GetAllocator()); //adding COLLISION member to json object


		rapidjson::Value valTextureObject(rapidjson::kStringType);

		valTextureObject.SetString(currentTexture.textureFileName->c_str(), currentTexture.textureFileName->size()); //setting filename of current texture.

		MainObject.AddMember("TEXTURE_FILE", valTextureObject, jsonDoc.GetAllocator()); //adding TEXTURE_FILE member so we can read from it, when we want.

		rapidjson::Value LISTObject(rapidjson::kObjectType);
		LISTObject.SetObject();

		rapidjson::Value & textureStringName = *new rapidjson::Value(rapidjson::kStringType);
		std::string & textureName = *new std::string; //this is just weird, but yeah it works. I could also just use a pointer, but now we don't have to use *
		textureName = std::string{ "TEXTURE_" }.append(std::to_string(texture_iterator));
		textureStringName.SetString(textureName.c_str(), textureName.size());

		for (mapObjectItem currentSprite : allSprites)
		{
			if (currentTexture.theTexture == currentSprite.objectSprite.getTexture()){ //when current texture equals texture used in sprite, we add this to the current texture list.
				rapidjson::Value objValue;
				objValue.SetObject();
				objValue.AddMember("layer", (int)currentSprite.layer, allocator); //writing all the information of the sprite.
				objValue.AddMember("posX", (int)currentSprite.objectSprite.getPosition().x, allocator);
				objValue.AddMember("posY", (int)currentSprite.objectSprite.getPosition().y, allocator);

				objValue.AddMember("width", (int)currentSprite.objectSprite.getGlobalBounds().width, allocator);
				objValue.AddMember("height", (int)currentSprite.objectSprite.getGlobalBounds().height, allocator);
				objValue.AddMember("scale", (float)currentSprite.objectSprite.getScale().x, allocator);
				objValue.AddMember("rotation", (int)currentSprite.objectSprite.getRotation(), allocator);

				LISTObject.AddMember("texture_item", objValue, allocator);
			}
		}

		MainObject.AddMember("LIST", LISTObject, jsonDoc.GetAllocator()); //adding list of all the texture_items




		allJSONTextures.AddMember(textureStringName, MainObject, jsonDoc.GetAllocator());

		texture_iterator++; //so we can get different names.
	}


	jsonDoc.AddMember("TEXTURES", allJSONTextures, jsonDoc.GetAllocator()); //adding COLLISION member to json object

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();  //
	std::ofstream out(levelName); //this will output JSON minified, but enough tools to get a logic order (or use visual studio).
	out << jsonString;
	out.close();

}



void Level::addSprite(sf::Sprite & theSprite, int layer, bool collision, sf::Texture & texture, std::string textureFileName){

	bool textureWasAlreadyHere = false; //we don't expect texture already to be already here.
	for_each(
		allTextures.begin(), 
		allTextures.end(), 
		[&](const texureObjectItem tt)
		{ 
			if (*tt.textureFileName == textureFileName){ //search for already existing texture, before adding one
				theSprite.setTexture(*tt.theTexture);
				textureWasAlreadyHere = true;
				return; //stopping loop, because it's not needed to loop further.
			}
		}
	);

	if (!textureWasAlreadyHere){ //when texture is new
		theSprite.setTexture(texture);
		texureObjectItem tt;
		tt.collision = collision;
		tt.textureFileName = new std::string{ textureFileName };
		tt.theTexture = &texture;
		allTextures.push_back(tt); //adding textures to texture list
	}

	mapObjectItem currentMapObjectItem; //creating sprite struct.
	currentMapObjectItem.layer = layer;
	currentMapObjectItem.collision = collision;
	currentMapObjectItem.objectSprite = theSprite;
	allSprites.push_back(currentMapObjectItem); //adding to main list.
	

	//again sorting by layer
	resortSprites();
}

sf::Sprite Level::createSprite(sf::Texture & usingTexture, int posX, int posY, int height, int width, int rotation, double scale){
	sf::Sprite sprite;
	sprite.setTexture(usingTexture);
	sprite.setPosition(posX, posY);
	sprite.setRotation(rotation);
	sprite.setScale(sf::Vector2f{ (float)scale, (float)scale });
	sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	return sprite;
}

void Level::draw(sf::RenderWindow & window){
	for (const mapObjectItem spriteAndTexture : allSprites){
		window.draw(spriteAndTexture.objectSprite);
	}
}

bool Level::overlappingPosition(sf::FloatRect unpenetrable){
	if (!allSprites.empty()){
		for (const mapObjectItem currentSprite : allSprites){
			if (currentSprite.collision){
				if (currentSprite.objectSprite.getGlobalBounds().intersects(unpenetrable)){	// BE SURE THE MOVABLE IS ACTUALLY ALSO A SPRITE
					return true;
				}
			}
		}
	}
	return false;
}