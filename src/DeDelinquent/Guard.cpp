#include "Guard.hpp"
#include "IDrawableComponent.hpp"
#include "Movable.hpp"
#include "StateController.hpp"

const std::string Guard::guardImageFilename = GlobalValues::MAIN_PATH() + "images/bewakerTextureAll.png";
const std::string Guard::guardsSaveFile = GlobalValues::MAIN_PATH() + "levels/guard_save_locations.json";

Guard::Guard(std::vector<sf::Vector2f> WayPointList, int lvlnm, sf::Vector2f startPosition):
startGuardPosition{startPosition},
patrolWaypoints( WayPointList ),
levelNumber{ lvlnm }
{
	startGuardPosition = startPosition;
	IDrawableComponent::pos = startPosition;
	Movable::setDrawing(guardImageFilename);
	setSpeed(speedOfGuard);
}

Guard::~Guard(){

}

sf::Vector2f Guard::getPosition(){
	return pos;
}

std::vector<sf::Vector2f> Guard::getPatrolWaypoints(){
	return patrolWaypoints;
}

std::list<Guard::waypoint> * Guard::getWaypointlist(){
	return &waypointsList;
}

void Guard::update(){
	updateAI();
	//if its waypointlist is empty it will add a new way point(stand still)
	lock.lock();
	if (waypointsList.empty()){
		waypointsList.emplace_front(waypoint{ sf::Vector2f{ 0, 0 }, false, false }); //waypoint is zero, because Guard doesn't move anymore.
	}
	setRunning(waypointsList.begin()->running);
	setSneaking(waypointsList.begin()->sneaking);
	if (Movable::move(waypointsList.front().direction))	{
			waypointsList.pop_front();
	}

	lock.unlock();
	
}

bool Guard::overlappingPosition(sf::FloatRect unpenetrable){
	if (Movable::theMovingImage.getGlobalBounds().intersects(unpenetrable)){
		StateController::drawSplashScreen = true;
		StateController::setActiveState(StateController::GAME_OVER);
	}
	return false;
}

void Guard::addPatrolWaypoint(sf::Vector2f patrolPoint){
	patrolWaypoints.emplace_back(patrolPoint);
}

int Guard::getLevelNumber(){
	return levelNumber;
}

std::vector<std::shared_ptr<Guard>> Guard::loadGuards(int lvlnm, bool inLevelEditor)
{
	std::vector<std::shared_ptr<Guard>> readGuards;
	std::ifstream infile(guardsSaveFile);
	if (!infile.good()) {
		std::ofstream out(guardsSaveFile);
		out << "{\n\n}";
		out.close();
	}
	else {

		rapidjson::Document * guardList = DiskController::readFileIntoJson(guardsSaveFile);

		for (rapidjson::Value::MemberIterator currentGuard = (*guardList).MemberBegin(); currentGuard != (*guardList).MemberEnd(); ++currentGuard) {

			int levelNumber = currentGuard->value["LEVEL"].GetInt();
			
				int posx = currentGuard->value["POSX"].GetInt();
				int posy = currentGuard->value["POSY"].GetInt();
				sf::Vector2f startPos = sf::Vector2f{ (float)posx, (float)posy };

				std::vector<sf::Vector2f> savedWayPoints;
				for (rapidjson::Value::MemberIterator guardWaypoint = currentGuard->value["PATROL_POINTS"].MemberBegin(); guardWaypoint != currentGuard->value["PATROL_POINTS"].MemberEnd(); ++guardWaypoint) {
					int posx = guardWaypoint->value["posX"].GetInt();
					int posy = guardWaypoint->value["posY"].GetInt();
					sf::Vector2f coordi = sf::Vector2f{ (float)posx, (float)posy };
					savedWayPoints.push_back(coordi);
				}
				if (!inLevelEditor){
					if (levelNumber == lvlnm){
						std::shared_ptr<Guard> createdGuard = std::shared_ptr<Guard>(new Guard{ savedWayPoints, levelNumber, startPos });
						readGuards.push_back(createdGuard);
					}
				}
				else{
					std::shared_ptr<Guard> createdGuard = std::shared_ptr<Guard>(new Guard{ savedWayPoints, levelNumber, startPos });
					readGuards.push_back(createdGuard);
				}
					
			
		}

		if (guardList != nullptr) {
			delete guardList;
			guardList = nullptr;
		}
	}
	return readGuards;
}

void Guard::saveGuards(const std::vector<std::shared_ptr<Guard>>& guards){
	
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();


	for (auto currentGuard : guards){
		if (currentGuard != nullptr){
			rapidjson::Value objValue(rapidjson::kObjectType);
			objValue.SetObject();

			objValue.AddMember("LEVEL", currentGuard->getLevelNumber(), allocator);
			objValue.AddMember("POSX", (int)currentGuard->getPosition().x, allocator);
			objValue.AddMember("POSY", (int)currentGuard->getPosition().y, allocator);

			rapidjson::Value WayPoints(rapidjson::kObjectType);
			WayPoints.SetObject();

			for (sf::Vector2f currentWaypoint : currentGuard->getPatrolWaypoints())
			{
		
				rapidjson::Value wwValue;
				wwValue.SetObject();
				wwValue.AddMember("posX", (int)currentWaypoint.x, allocator);
				wwValue.AddMember("posY", (int)currentWaypoint.y, allocator);


				WayPoints.AddMember("waypoint", wwValue, allocator);

			}
			objValue.AddMember("PATROL_POINTS", WayPoints, allocator);

			jsonDoc.AddMember("GUARD", objValue, jsonDoc.GetAllocator());
		}
	}

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();  //

	std::remove(guardsSaveFile.c_str()); // std::remove() doesn't accept const string
	std::ofstream out(guardsSaveFile);
	out << jsonString;
	out.close();

}

void Guard::resetGuards(std::vector<std::shared_ptr<Guard>> & guards, int levelNumber){
	guards.erase(
		std::remove_if(
		guards.begin(),
		guards.end(),
		[&](std::shared_ptr<Guard> theGuard)
	{
		return theGuard.get()->getLevelNumber() == levelNumber;
	}
		),
		guards.end()
		);

}