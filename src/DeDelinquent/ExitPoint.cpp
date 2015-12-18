#include "ExitPoint.hpp"
#include "StateController.hpp"

ExitPoint::ExitPoint(int currentLevelNumber, bool playerMode) : currentLevelNumber{ currentLevelNumber }, playerMode{playerMode}
{

	loadExitsFromFile();
	if (exitTexture.loadFromFile(exitPointTextureLocation) ) {
		exitTexture.setRepeated(false);
		exitTexture.setSmooth(true);
		exitSprite = sf::Sprite{ exitTexture };
	}
}

ExitPoint::~ExitPoint(){
	//nothing to clean up

}

void ExitPoint::loadExitsFromFile() {
	std::ifstream infile(exitPointSaveLocationList);
	if (!infile.good()) {
		std::ofstream out(exitPointSaveLocationList);
		out << "{\n\n}"; //creating empty json file
		out.close();
	}
	else {
		rapidjson::Document * exits = DiskController::readFileIntoJson(exitPointSaveLocationList);
		for (rapidjson::Value::MemberIterator m = (*exits).MemberBegin(); m != (*exits).MemberEnd(); ++m) {
			if (m->value.IsObject()) {
				for (rapidjson::Value::MemberIterator subitems = m->value.MemberBegin(); subitems != m->value.MemberEnd(); ++subitems) {
					int levelNumber = subitems->value["LEVEL"].GetInt();
					int posx = subitems->value["POSX"].GetInt();
					int posy = subitems->value["POSY"].GetInt();

					addExitPoint(sf::Vector2f{ (float)posx, (float)posy }, levelNumber);

				}
			}
		}
		delete exits;
	}
}

void ExitPoint::saveExitsToFile() {
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

	rapidjson::Value MainObject(rapidjson::kObjectType); //creating one head item for one texture including the specs
	MainObject.SetObject();
	
	for (exitPointAndLevelnumber & exit : exitPointsLocation) {
		rapidjson::Value objValue(rapidjson::kObjectType);
		objValue.SetObject();

		rapidjson::Value valTextureObject(rapidjson::kStringType);

		objValue.AddMember("LEVEL", exit.levelnumber, allocator);
		objValue.AddMember("POSX", (int)exit.exitObject.getPosition().x, allocator);
		objValue.AddMember("POSY", (int)exit.exitObject.getPosition().y, allocator);

		MainObject.AddMember("item", objValue, jsonDoc.GetAllocator());
	}

	jsonDoc.AddMember("EXIT", MainObject, jsonDoc.GetAllocator());

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();

	std::ofstream out(exitPointSaveLocationList);
	out << jsonString;
	out.close();
}

void ExitPoint::addExitPoint(sf::Vector2f exitLocation, int levelNumber){
	sf::Sprite & sp = sf::Sprite{};
	sp.setTexture(exitTexture);
	sp.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sp.setScale(sf::Vector2f{ 0.5, 0.5 });
	sp.setPosition(exitLocation);
	sp.setColor(sf::Color::Red);

	exitPointAndLevelnumber exPandLevel;
	exPandLevel.exitObject = sp;
	exPandLevel.levelnumber = levelNumber;

	exitPointsLocation.push_back(exPandLevel);
}

void ExitPoint::draw(sf::RenderWindow & window){ //draw only the level specificItems.
	if (!playerMode){
		for (const exitPointAndLevelnumber & exitPoint : exitPointsLocation){ //draw all exitpoints when in leveleditor
			if (currentLevelNumber == exitPoint.levelnumber){
				window.draw(exitPoint.exitObject);
			}
		}
	}
	else{
		if (PlayerData::getInstance().playerCanFinish()){ //player can see the exitpoint.
			for (const exitPointAndLevelnumber & exitPoint : exitPointsLocation){
				if (currentLevelNumber == exitPoint.levelnumber){
					window.draw(exitPoint.exitObject);
				}
			}
		}
	}
	
	
}

void ExitPoint::resetExitPoints(int levelNumber){
	exitPointsLocation.erase(
		std::remove_if(
		exitPointsLocation.begin(),
		exitPointsLocation.end(),
		[&](exitPointAndLevelnumber & expnt){
			if (levelNumber == expnt.levelnumber)
			{
				return true;
			}
			else{
				return false;
			}
		}
		),
		exitPointsLocation.end()

	);
}

bool ExitPoint::overlappingPosition(sf::FloatRect unpenetrable){
	if (PlayerData::getInstance().playerCanFinish()){
		
		for (exitPointAndLevelnumber & exit : exitPointsLocation) {
			if (exit.exitObject.getGlobalBounds().intersects(unpenetrable) && exit.levelnumber == currentLevelNumber) {
				PlayerData::getInstance().updateExperience(PlayerData::getInstance().getMainObjectiveExp());
				PlayerData::getInstance().playerMayFinish(false);


				if (GlobalValues::SAVE_PLAYER_DATA){
					PlayerData::getInstance().setLevelNumber(PlayerData::getInstance().getLevelNumber() + 1);
					PlayerData::getInstance().savePlayerDataToFile();
				}

				StateController::drawSplashScreen = true;
				StateController::setActiveState(StateController::LEVEL_FINISHED);

				return true;
			}
		}
	}
	return false;
}