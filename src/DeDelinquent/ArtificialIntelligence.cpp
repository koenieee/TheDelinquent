#include "ArtificialIntelligence.hpp"
#include "Player.hpp"
#include "PathFinding.hpp"
#include <iostream>
#include <algorithm>
#include <SFML\System\Thread.hpp>

float ArtificialIntelligence::awareness = 0;
int ArtificialIntelligence::awarenessLevel = 0;

std::vector<ArtificialIntelligence*> ArtificialIntelligence::allArtificialIntelligence = std::vector<ArtificialIntelligence * >();

ArtificialIntelligence::ArtificialIntelligence(){


	if (player != nullptr && level != nullptr){
		aiThread = std::shared_ptr<sf::Thread>(new sf::Thread(&ArtificialIntelligence::threadLoop, this));
		threadShouldRunning = true;
		aiThread->launch();
		addArtificialIntelligenceToSaveList(this);
	}
	else{
	}

}

std::shared_ptr<Player> ArtificialIntelligence::player = nullptr;

void ArtificialIntelligence::setPlayer(std::shared_ptr<Player> player1){
	player = player1;
}
std::shared_ptr<Level> ArtificialIntelligence::level = nullptr;
void ArtificialIntelligence::setLevel(std::shared_ptr<Level> setLevel){
	level = setLevel;
}

void ArtificialIntelligence::resetPlayerAndLevel(){
	level = nullptr;
	player = nullptr;
}

ArtificialIntelligence::~ArtificialIntelligence(){
	ArtificialIntelligence::allArtificialIntelligence.clear();
}

void ArtificialIntelligence::threadLoop() {
	while (true){
		if (!threadShouldRunning) {
			return;
		}
		if (tick){
			updateDistanceToPlayer();
			playerInSightUpdate();
			calculateNextPathFindingWaypoint();
			tick = false;
		}
		if (update){
			update = false;
			updateNow();
		}
		sf::sleep(sf::milliseconds(50));
	}
}




void ArtificialIntelligence::updateAI(){


		tick = true;
	if (updateInterval == 0){
		update = true;
	}
	else if (playerInSight && (updateInterval % (updateFreq / 2) == 0)){ update = true; }

	++updateInterval;
	updateInterval %= updateFreq;


}


void ArtificialIntelligence::updateNow(){


	if (playerInSight){
		shouldRun = true;
		setFinalTarget(player->getPos());
		suspicious = false;
	}
	else if (suspicious){
		shouldRun = false;
		setFinalTarget(suspiciousLocation);
		if (getFloatRect(getPosition()).contains(suspiciousLocation)){
			suspicious = false;
		}
	}
	else{
		shouldRun = false;
		if (!getPatrolWaypoints().empty()){
			setFinalTarget(getPatrolWaypoints().at(patrolWaypointTarget));
			if (getFloatRect(getPosition()).contains(getPatrolWaypoints().at(patrolWaypointTarget))){
				++patrolWaypointTarget;
				patrolWaypointTarget %= getPatrolWaypoints().size();
			}
		}


	}

}

void ArtificialIntelligence::calculateNextPathFindingWaypoint(){
	lock.lock();
	if (calculatePathFindingWaypoint){
		if (!pathFindingWaypoints.empty()){
			if (!setNextTarget(pathFindingWaypoints.at(calculateNextPathFindingWaypointIterator), true)){}
			++calculateNextPathFindingWaypointIterator;
			if (calculateNextPathFindingWaypointIterator >= pathFindingWaypoints.size()){ calculatePathFindingWaypoint = false; }
		}

	}
	lock.unlock();
}


void ArtificialIntelligence::setFinalTarget(sf::Vector2f finalTar){


	lock.lock();
	getWaypointlist()->clear();


	if (setNextTarget(finalTar)){
		lock.unlock();
	}
	else{
		lock.unlock();
		sf::sleep(sf::milliseconds(10));
		static PathFinding pathFinding = PathFinding(ArtificialIntelligence::level);
		try{

			calculatePathFindingWaypoint = true;
			calculateNextPathFindingWaypointIterator = 0;
			pathFindingWaypoints = pathFinding.calculatePath(getPosition(), finalTar);
			std::reverse(pathFindingWaypoints.begin(), pathFindingWaypoints.end());
			calculateNextPathFindingWaypoint();
		}
		catch (std::exception){
			resetAllPossableTargets();
			return;
		}
	}
}


bool ArtificialIntelligence::setNextTarget(sf::Vector2f nextTar, bool calledByPathFinding){
	try{
		getWaypointlist()->splice(getWaypointlist()->end(), translate2Directions(calculateLastPos(), nextTar, shouldRun, shouldSneak, calledByPathFinding));
		return true;
	}
	catch (int){
		return false;
	}
}



std::list<ArtificialIntelligence::waypoint> ArtificialIntelligence::translate2Directions(sf::Vector2f start, sf::Vector2f end, bool isRunning, bool isSneaking, bool calledByPathFinding){
	std::list<waypoint> tempWaypointsList;
	sf::Vector2f realTimePos = start;
	sf::Vector2f tempDirection = { 0, 0 };
	float speed = 2;

	int size = 0;

	while (!getFloatRect(realTimePos).contains(end)){
		tempDirection = { 0, 0 };

		if ((realTimePos.x < end.x) && !(realTimePos.x + speed > end.x)){
			++tempDirection.x;
		}
		else if (realTimePos.x > end.x){
			--tempDirection.x;
		}

		if ((realTimePos.y < end.y) && !(realTimePos.y + speed > end.y)){
			++tempDirection.y;
		}
		else if (realTimePos.y > end.y){
			--tempDirection.y;
		}

		sf::Vector2f directionToPushBack = tempDirection;
		if (!canMovetowards(realTimePos, &tempDirection, isRunning, isSneaking)){
			if (!calledByPathFinding){
				throw 1;
			}
		}
		if (++size > 100 && calledByPathFinding){
			resetAllPossableTargets();
			return std::list < ArtificialIntelligence::waypoint > {};
		}

		realTimePos = realTimePos + tempDirection;

		tempWaypointsList.push_back(waypoint(directionToPushBack, isRunning, isSneaking));


	}
	return tempWaypointsList;
}

sf::Vector2f ArtificialIntelligence::calculateLastPos(){
	std::list<waypoint> * waypointList = getWaypointlist();
	sf::Vector2f lastPos = getPosition();
	float speed = 2;
	float newSpeed;
	static float diagmuliplier = 1 / sqrt(2);


	for (std::list<waypoint>::const_iterator iterator = waypointList->begin(), end = waypointList->end(); iterator != end; ++iterator){
		newSpeed = speed;
		if (abs(iterator->direction.x) != abs(iterator->direction.y)){ newSpeed *= diagmuliplier; }
		if (iterator->running){ newSpeed *= 2; }
		if (iterator->sneaking){ newSpeed *= 0.5; }
		lastPos += iterator->direction*newSpeed;
	}
	return lastPos;

}



void ArtificialIntelligence::playerInSightUpdate(){
	float tempVisionRange = visionRange*(1 + awareness);

	if (distanceToPlayer < MinimalVisionRange){
		playerInSightTime = 1000;
	}
	else if (distanceToPlayer < tempVisionRange){

		float tempIncreasementSteps = 1;
		if (playerInSightTime < 1000){
			tempIncreasementSteps = (distanceToPlayer / 100)*((1 + awareness) / 50)*stepSizeMultiplierOfSeeing;
			playerInSightTime = playerInSightTime + tempIncreasementSteps;
		}
	}
	else{

		if (playerInSightTime > 1){
			playerInSightTime = playerInSightTime - stepSizeOfForgetting;
		}
	}

	if (playerInSightTime > playerInSightThreshold){
		if (!playerInSight){
			playerInSight = true;
			updateNow();
		}
		else{
		}
	}
	else{
		playerInSight = false;
	}
}

sf::FloatRect ArtificialIntelligence::getFloatRect(sf::Vector2f pos){
	return sf::Rect< float >::Rect(pos - sf::Vector2f{ 5, 5 }, { 50.0f, 50.0f });
}


ArtificialIntelligence* ArtificialIntelligence::getSmallestDistanceGuard(){
	float temp = 10000;
	ArtificialIntelligence* temp1 = nullptr;
	for (ArtificialIntelligence * i : ArtificialIntelligence::allArtificialIntelligence) {
		if (i->distanceToPlayer < temp){
			temp = i->distanceToPlayer;
			temp1 = i;
		}
	}
	return temp1;
}

void ArtificialIntelligence::addArtificialIntelligenceToSaveList(ArtificialIntelligence * addedAI){
	allArtificialIntelligence.push_back(addedAI);
}

void ArtificialIntelligence::setAwarenessLevel(int awarLvl){
	if (awarenessLevel != awarLvl){
		if (awarLvl == 2){
			auto selectedGuard = getSmallestDistanceGuard();
			if (!selectedGuard->playerInSight){
				selectedGuard->suspicious = true;
				selectedGuard->suspiciousLocation = player->getPos();
			}
		}
		if (awarLvl == 1){
		}
		if (awarLvl == 3){
			getSmallestDistanceGuard()->playerInSightTime = 1000;

		}

	}
	awarenessLevel = awarLvl;
}

float ArtificialIntelligence::getSmallestDistance(){
	float temp = 10000;
	for (ArtificialIntelligence * i : ArtificialIntelligence::allArtificialIntelligence) {
		if (i->distanceToPlayer < temp){
			temp = i->distanceToPlayer;
		}
	}
	return temp;
}

void ArtificialIntelligence::updateDistanceToPlayer(){
	distanceToPlayer = sqrt(pow((player->getPos().x) - (getPosition().x), 2) + pow((player->getPos().y) - (getPosition().y), 2));
}

void ArtificialIntelligence::resetAllPossableTargets(){
	suspicious = false;
	playerInSight = false;
	++patrolWaypointTarget;
	patrolWaypointTarget %= getPatrolWaypoints().size();
}

void ArtificialIntelligence::stopAllArtificialIntelligenceThreads(){
	for (ArtificialIntelligence * i : ArtificialIntelligence::allArtificialIntelligence) {
		if (i->aiThread != nullptr) {
			i->threadShouldRunning = false;
			i->aiThread->wait();
			i->aiThread->terminate();
			i->aiThread = nullptr;
		}
	}
}