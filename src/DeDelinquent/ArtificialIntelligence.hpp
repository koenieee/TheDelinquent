#pragma once
#include <SFML\Graphics.hpp>
#include <list>
#include "Player.hpp"
#include <iostream>
#include <mutex>
#include <memory>

/**
*
* @version 4.0
*
* An guard has its own AI object, and its own thread.
* There will be static attributes and methods for things that will be concern all ArtificialIntelligence.
*
*/

class ArtificialIntelligence {
public:

	/**
	* Sets an player for all AI
	* @param player1 is an shared_ptr of player so it can ask for his current position
	*/
	static void ArtificialIntelligence::setPlayer(std::shared_ptr<Player> player1);

	/**
	* Sets an level for all AI.
	* From the level it will calculated Dijkstra's way points.
	* @param setLevel is an shared_ptr where the AI in walks.
	*/
	static void ArtificialIntelligence::setLevel(std::shared_ptr<Level> setLevel);

	/**
	* Sets nullPtr for player and level.
	*/
	static void ArtificialIntelligence::resetPlayerAndLevel();

	/**
	* Contains directions that the guard(movable) can follow.
	* It mimics user input for one game tick.
	*/
	struct waypoint
	{
		waypoint(sf::Vector2f vec, bool run, bool sneak){
			direction = vec;
			running = run;
			sneaking = sneak;
		}
		sf::Vector2f direction{ 0, 0 };		//this should only have values like {-1  0  1}
		bool running = false;
		bool sneaking = false;
	};

	/**
	* Stops all thread for all the ArtificialIntelligence, this is called before destructing any parts that could cause an error.
	*/
	static void ArtificialIntelligence::stopAllArtificialIntelligenceThreads();

	/**
	* Gets the distance between the closest guard and player.
	* It does not calculate the distance, thats done by the AI Thread.
	*/
	static float ArtificialIntelligence::getSmallestDistance();

	/**
	* Sets an awareness level for the guards.
	* It will check wheater it has changed, and if it has it will update the state an guard is in.
	* Level 1: nothing is going on, all guards need to patrol.
	* Level 2: something might not be right, send closest guard to inspect player's location when level was set.
	* Level 3: something is not right, send closest guard to run towards players position.
	* @param awarLvl an integer between 1-3
	*/
	static void ArtificialIntelligence::setAwarenessLevel(int awarLvl);

private:

	//Tweakables that effect how guards behave.
	const float ArtificialIntelligence::visionRange{ 10.0f };//this gets multiplied with awareness
	static const int ArtificialIntelligence::updateFreq{ 200 };
	const float ArtificialIntelligence::MinimalVisionRange{ 80.0f };//will override PlayerInSight if player is within minimalVisionRange...
	const float playerInSightThreshold = 500.0f;
	const float stepSizeOfForgetting = 5.0f;
	const float stepSizeMultiplierOfSeeing = 1.0f;


	void ArtificialIntelligence::addArtificialIntelligenceToSaveList(ArtificialIntelligence * addedAI);

	void resetAllPossableTargets();
	
	static ArtificialIntelligence* ArtificialIntelligence::getSmallestDistanceGuard();

	static std::vector<ArtificialIntelligence*> ArtificialIntelligence::allArtificialIntelligence;

	//Local used states for AI.
	bool shouldRun = false;
	bool shouldSneak = false;
	bool playerInSight = false;
	bool suspicious = false;

	//Local used variables for (all) AI.
	static float awareness;
	static int awarenessLevel;
	float playerInSightTime = 0;
	float distanceToPlayer;
	static std::shared_ptr<Player> player;
	static std::shared_ptr<Level> level;
	sf::Vector2f finalTarget;
	std::vector<sf::Vector2f> pathFindingWaypoints;
	std::vector<sf::Vector2f> targetList;
	sf::Vector2f suspiciousLocation;

	//Attributes that manages the updates of AI.
	int updateInterval = rand()%updateFreq;//Generates an random numer to start, this is so the guards are less likely to have the same update phase.
	bool tick = false;
	bool update = false;
	bool calculatePathFindingWaypoint = false;
	bool threadShouldRunning;
	std::shared_ptr<sf::Thread> aiThread;
	unsigned int calculateNextPathFindingWaypointIterator = 0;
	unsigned int patrolWaypointTarget = 0;

	virtual sf::Vector2f ArtificialIntelligence::getPosition() = 0;

	void ArtificialIntelligence::threadLoop();

	void ArtificialIntelligence::updateDistanceToPlayer();

	/**
	* Adds next path finding way-point to directions list.
	*/
	void ArtificialIntelligence::calculateNextPathFindingWaypoint();

	/**
	* Translate an position to direction, will throw an exception if its not possible.
	*/
	std::list<waypoint> ArtificialIntelligence::translate2Directions(sf::Vector2f start, sf::Vector2f end, bool isRunning, bool isSneaking, bool calledByPathFinding = false);


	/**
	* This is an what wider hit-box for the guard to check if got to his target.
	*/
	sf::FloatRect ArtificialIntelligence::getFloatRect(sf::Vector2f pos);

	/*
	* Calculates the last position of the waypointList of guard.
	*/
	sf::Vector2f ArtificialIntelligence::calculateLastPos();
	
	void ArtificialIntelligence::playerInSightUpdate();

	/*
	* Clears waypointlist, tries to find an direct path.
	* If that fails it will calculate an path with pathFinding and translates the the first node into directions.
	*/
	void ArtificialIntelligence::setFinalTarget(sf::Vector2f finalTar);

	/*
	* Does not clear waypointList, but will add new directions after the last way-point.
	*/
	bool ArtificialIntelligence::setNextTarget(sf::Vector2f nextTar, bool calledByPathFinding = false);

	/*
	* This contains the logic for its Intelligence.
	*/
	void ArtificialIntelligence::updateNow();

protected:
	/**
	* An mutex for locking non atomic values uses by the main thread and the AI thread.
	*/
	std::mutex lock;

	/**
	* Constructor for the AI.
	* Checks if its ready for constructing and launches the AI Thread and stores himself in an static container.
	*/
	ArtificialIntelligence::ArtificialIntelligence();

	/**
	* Destructs the AI(NOT THE THREAD).
	*/
	ArtificialIntelligence::~ArtificialIntelligence();

	/**
	* Waypoints(directions).
	* @see waypoint
	* @see Guard::update()
	* @return a pointer of a list of waypoints.
	*/
	virtual std::list<waypoint>* ArtificialIntelligence::getWaypointlist() = 0;

	/**
	* @return vector of patrol coordinates(waypoints)
	*/
	virtual std::vector<sf::Vector2f> ArtificialIntelligence::getPatrolWaypoints() = 0;

	/**
	* @see movable::canMovetowards 
	*/
	virtual bool canMovetowards(sf::Vector2f position, sf::Vector2f * moveTowards, bool isRunning, bool isSneaking) = 0;

	/*
	* This will update one time per 'updateFreq'. its called every game tick. when its time to update, it sets an bool true so the AI thread will call updateNow().
	* @see ArtificialIntelligence::updateNow()
	*/
	void ArtificialIntelligence::updateAI();
	
};
