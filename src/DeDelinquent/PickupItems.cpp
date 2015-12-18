#include "PickupItems.hpp"
#include "State.hpp"


PickupItems::PickupItems(int levelNM, int totalExperienceLevelPoints)
{
	currentLevelNumber = levelNM;
	loadPickupsFromFile();
	levelSpecificPickups = getLevelSpecificItems(currentLevelNumber);
	maxLevelExperience = totalExperienceLevelPoints;
	
	// Create a text
	theFont = std::shared_ptr<sf::Font>(new sf::Font{ *State::createFont() }); //load font one time only from State.

	notifcationText.setFont(*theFont);
	notifcationText.setCharacterSize(35);
	notifcationText.setString(playerFoundItem);
	notifcationText.setStyle(sf::Text::Italic);
	notifcationText.setColor(sf::Color::Red);

	clock.restart();

}

PickupItems::~PickupItems(){

	for (auto & lvlspc : allPickupItems){
		if (lvlspc != nullptr) {
			if (lvlspc->pickupSprite != nullptr) {
				delete lvlspc->pickupSprite;
				lvlspc->pickupSprite = nullptr;
			}
			if (lvlspc->pickupTexture != nullptr) {
				delete lvlspc->pickupTexture;
				lvlspc->pickupTexture = nullptr;
			}
			if (lvlspc->pickupTextureFilename != nullptr) {
				delete lvlspc->pickupTextureFilename;
				lvlspc->pickupTextureFilename = nullptr;
			}
			lvlspc = nullptr;
		}
	}
	if (theFont != nullptr){
		theFont = nullptr;
	}


	levelSpecificPickups.clear();
	allPickupItems.clear();
}

void PickupItems::addPickupItem(bool isMainObject, int levelNumber, std::string textFileName, sf::Vector2f findLocation, bool playerPickupedItem, bool itemPlacedInLevel){
	bool continueAddingItem = true;
	for_each(
		levelSpecificPickups.begin(),
		levelSpecificPickups.end(),
		[&](const std::shared_ptr<pickupItem> & pi){
			if (textFileName == *pi->pickupTextureFilename && pi->itemPlacedInLevel == false)
			{
				continueAddingItem = false;
			}
		}
	);
	
	if (continueAddingItem){
	
		sf::Texture *pickupTexture  = new sf::Texture;
		if (!pickupTexture->loadFromFile(textFileName))
		{
			throw std::exception("Couldn't load a texture");
		}

		pickupTexture->setSmooth(true); //making smootie of texture

		sf::Sprite * pickupSprite = new	sf::Sprite;
		pickupSprite->setTexture(*pickupTexture);
		if (findLocation != sf::Vector2f{ 0, 0 }){ //start location is zero.
			pickupSprite->setPosition(findLocation);
		}
		pickupSprite->setScale(sf::Vector2f{ (float)0.5, (float)0.5 }); //pickup items are scaled to 0.5
		pickupSprite->setRotation(0);

		std::shared_ptr<pickupItem> pickup = std::shared_ptr<pickupItem>(new pickupItem);
		pickup->mainObjectItem = isMainObject;
		pickup->playerPickupedItem = playerPickupedItem;
		pickup->levelNumber = levelNumber;
		pickup->itemPlacedInLevel = itemPlacedInLevel;
		pickup->pickupSprite = pickupSprite;
		pickup->pickupTexture = pickupTexture;
		pickup->pickupTextureFilename = new std::string{ textFileName };

		allPickupItems.push_back(pickup);
	}
}


std::shared_ptr<PickupItems::pickupItem> PickupItems::checkForPickUpCheck(sf::Vector2i mousePos){ //only used in levelEditor
	
	for (auto & pickup : allPickupItems){
		if (pickup->levelNumber == currentLevelNumber){
			if (pickup->pickupSprite->getGlobalBounds().contains((sf::Vector2f)mousePos)){
				return pickup;
			}
		}
	}
	return nullptr;
}


void PickupItems::draw(sf::RenderWindow & window){ //draw only the level specificItems.
	mutex.lock();
	for (const auto & pp : levelSpecificPickups){
		if (pp->itemPlacedInLevel){
			if (!pp->playerPickupedItem){
				window.draw(*pp->pickupSprite);
			}
		}
	}
	if (renderNotification) {
		notifcationText.setPosition(notifTextLocation);
		if (!(clock.getElapsedTime().asSeconds() >= PickupItems::textSeconds))
		{
			sf::Color col = notifcationText.getColor();
			float elapsedTime = clock.getElapsedTime().asSeconds();
			col.a = elapsedTime == 0.0f ? 255 : 255 - 255 / (PickupItems::textSeconds / elapsedTime);
			notifcationText.setColor(col);
			window.draw(notifcationText);
		}
		else {
			renderNotification = false;
		}
	}
	mutex.unlock();
}

void PickupItems::resetPickupItems(int levelNumber){
	for_each(
		allPickupItems.begin(),
		allPickupItems.end(),
		[&](std::shared_ptr<pickupItem> & pi){
		if (levelNumber == pi->levelNumber)
		{
			pi->playerPickupedItem = false;
			pi->itemPlacedInLevel = false;
			pi->pickupSprite->setPosition(sf::Vector2f{ 0, 0 });
		}
	});
}

int PickupItems::calculateExperiencePoints(){
	
	int subItemsXPPercentage = maxLevelExperience*0.2; //sub items are 20% of the total level experience.
	int mainItemsXPPercentage = maxLevelExperience*0.8; //so the Main Item is 80% of the total level exerperience.
	int numberOfSubItems = std::count_if(levelSpecificPickups.begin(), levelSpecificPickups.end(), [](const std::shared_ptr<pickupItem> & pickup) {return pickup->itemPlacedInLevel && !pickup->mainObjectItem; }); //check if item is placed in level, and if item is not a main objective.


	int playerFoundItems = std::count_if(
		levelSpecificPickups.begin(),
		levelSpecificPickups.end(),
		[&](const std::shared_ptr<pickupItem> & pickup)
		{
			if (!pickup->mainObjectItem && pickup->itemPlacedInLevel){
				return pickup->playerPickupedItem;
			}
			else{
				return false;
			}
		}
	); 

	float percantageExpPoints = (float)playerFoundItems / (float)numberOfSubItems;
	int temp = subItemsXPPercentage* percantageExpPoints+ mainItemsXPPercentage;
	
	return temp;
	
}

std::vector<std::shared_ptr<PickupItems::pickupItem>> PickupItems::getLevelSpecificItems(int levelNumber){
	std::vector<std::shared_ptr<pickupItem>> selectedPickups;

	for (auto & currentPickup : allPickupItems){
		if (currentPickup->levelNumber == levelNumber){
			selectedPickups.push_back(currentPickup);
		}
	}
	return selectedPickups;
}

void PickupItems::loadPickupsFromFile(){


	std::ifstream infile(pickupItemSaveLocationList);
	if (!infile.good()){ //if file doesn't exists create empty level, only containting {}, otherwise rapidjson will fail.
		std::ofstream out(pickupItemSaveLocationList);
		out << "{\n\n}";
		out.close();
	}
	else{

		rapidjson::Document * pickups = DiskController::readFileIntoJson(pickupItemSaveLocationList);

		for (rapidjson::Value::MemberIterator m = (*pickups).MemberBegin(); m != (*pickups).MemberEnd(); ++m) { //reading texture settings located in a directory and a file.
		
			if (m->value.IsObject()){
				for (rapidjson::Value::MemberIterator subitems = m->value.MemberBegin(); subitems != m->value.MemberEnd(); ++subitems) {

					bool mainitem = subitems->value["MAINITEM"].GetBool();
					bool playerPickupItem = subitems->value["PLAYER_PICKUPED_ITEM"].GetBool();
					bool itemPlacedInLevel = subitems->value["ITEM_PLACED_IN_LEVEL"].GetBool();
					std::string * textureFileName = new std::string{ subitems->value["FILE"].GetString() };
					int levelNumber = subitems->value["LEVEL"].GetInt();
					int posx = subitems->value["POSX"].GetInt();
					int posy = subitems->value["POSY"].GetInt();

					addPickupItem(mainitem, levelNumber, *textureFileName, sf::Vector2f{ (float)posx, (float)posy }, playerPickupItem, itemPlacedInLevel);

				
				}
			}
		}
		delete pickups;
	}
}

void PickupItems::savePickupsToFile(){

	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

	rapidjson::Value MainObject(rapidjson::kObjectType); //creating one head item for one texture including the specs
	MainObject.SetObject();



	for (auto & currentPickup : allPickupItems){

			rapidjson::Value objValue(rapidjson::kObjectType);
			objValue.SetObject();

			rapidjson::Value valTextureObject(rapidjson::kStringType);

			valTextureObject.SetString(currentPickup->pickupTextureFilename->c_str(), currentPickup->pickupTextureFilename->size()); //setting filename of current texture.

			objValue.AddMember("FILE", valTextureObject, allocator); //writing all the information of the sprite.
			objValue.AddMember("LEVEL", currentPickup->levelNumber, allocator);
			objValue.AddMember("MAINITEM", currentPickup->mainObjectItem, allocator);
			objValue.AddMember("PLAYER_PICKUPED_ITEM", currentPickup->playerPickupedItem, allocator);
			objValue.AddMember("ITEM_PLACED_IN_LEVEL", currentPickup->itemPlacedInLevel, allocator);
			objValue.AddMember("POSX", (int)currentPickup->pickupSprite->getPosition().x, allocator);
			objValue.AddMember("POSY", (int)currentPickup->pickupSprite->getPosition().y, allocator);

			MainObject.AddMember("item", objValue, jsonDoc.GetAllocator());
	}
	
	jsonDoc.AddMember("PICKUPS", MainObject, jsonDoc.GetAllocator());
	
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();  //

	std::ofstream out(pickupItemSaveLocationList); //this will output JSON minified, but enough tools to get a logic order (or use visual studio).
	out << jsonString;
	out.close();
}

bool PickupItems::overlappingPosition(sf::FloatRect unpenetrable){
	
	for (auto & currentPickup : levelSpecificPickups){
		if (currentPickup->pickupSprite->getGlobalBounds().intersects(unpenetrable) && !currentPickup->playerPickupedItem && currentPickup->itemPlacedInLevel){	// BE SURE THE MOVABLE IS ACTUALLY ALSO A SPRITE
			mutex.lock();
			
			currentPickup->playerPickupedItem = true; //player has pickuped item.
			clock.restart();
			notifTextLocation = currentPickup->pickupSprite->getPosition();
			renderNotification = true;

			if (currentPickup->mainObjectItem){
				notifcationText.setString(playerFoundMainItem);
				int exp = calculateExperiencePoints();
				PlayerData::getInstance().mainObjectiveExp(exp);
				PlayerData::getInstance().playerMayFinish(true);

			}
			mutex.unlock();
			return true;

		}
	}
	
	return false;
}


