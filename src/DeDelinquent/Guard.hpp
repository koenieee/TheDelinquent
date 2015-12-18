#pragma once


#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "IDrawableComponent.hpp"
#include "UpdateableComponent.hpp"
#include "Movable.hpp"
#include "Level.hpp"
#include <forward_list>
#include <iostream>
#include <list>
#include <array>
#include <fstream>
#include "ArtificialIntelligence.hpp"
#include "UnpenetrableComponent.hpp"
#include <mutex>
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include <memory>


/**
* Guard is a level object that can be seen as the main enemy of the Player.
* @version 1.0
*/

class Guard : public Movable, public UpdateableComponent, public ArtificialIntelligence, public UnpenetrableComponent{

public:


	/**
	* Guard is initialized here using a the waypoint list, and it's connected to a loaded level. The start position can also be set.
	* @param WayPointList The first waypoints of the guards, this list can be empty, when you later want to add waypoints use the AddWaypointFunction
	* @param levelNumber The levelnumber is used to connect a guard to a level
	* @param startPosition The first start position the guard will be drawn.
	*/
	Guard::Guard(std::vector<sf::Vector2f> WayPointList, int levelNumber, sf::Vector2f startPosition = sf::Vector2f{ 200, 150 });

	/**
	* Guard is destructed, this also clears all pointers and other private parts.                                                                     
	*/
	Guard::~Guard();


	sf::Vector2f  ArtificialIntelligence::getPosition() override;


	void UpdateableComponent::update() override;


	bool UnpenetrableComponent::overlappingPosition(sf::FloatRect unpenetrable) override;


	std::list<waypoint>* ArtificialIntelligence::getWaypointlist() override;

	/**
	* Used to add another patrol waypoint to the guard.
	* @param patrolPoint The waypoint the guard will walk to.
	*/
	void Guard::addPatrolWaypoint(sf::Vector2f patrolPoint);

	/**
	* Returns the current level number the guards is connected to.
	* @return The current level number.
	*/
	int Guard::getLevelNumber();

	
	/**
	* Static method to reset the specified list of guards, connected to the levelNumber.
	* @param guards A vector list containing Guard pointers
	* @param levelNumber The levelnumber connected 
	*/
	static void Guard::resetGuards(std::vector<std::shared_ptr<Guard>> & guards, int levelNumber);

	/**
	* Static method to load, create and return all the guards found the in the JSON file.
	* @param levelNumber The guards connected to the level number you want to retrieve
	* @param inLevelEditor Value only used in levelEditor to retrieve all guards, instead of combining them to a levelnumber.
	* @return A list containing all the created Guards as shared pointers.
	*/
	static std::vector<std::shared_ptr<Guard>> Guard::loadGuards(int levelNumber, bool inLevelEditor = true);

	/**
	* Method to save all the guards to a JSON file
	* @param guards A list of guards with all the elements of a Guards, such as levelNumber, start Location and waypoints.
	*/
	static void Guard::saveGuards(const std::vector<std::shared_ptr<Guard>>& guards);

	/**
	* Determine if the guard can move towards the given position.                                                                      
	*/
	bool Guard::canMovetowards(sf::Vector2f position, sf::Vector2f * moveTowards, bool isRunning, bool isSneaking) override {
		return canMove(position, moveTowards, isRunning, isSneaking);
	}

private:

	float speedOfGuard=2.0f;
	int levelNumber;

	sf::Vector2f startGuardPosition;

	static const std::string guardImageFilename;
	static const std::string guardsSaveFile;
	
	int awarenessLevel = 0;
	
	std::list<waypoint> waypointsList;


	std::vector<sf::Vector2f> getPatrolWaypoints() override;

	std::vector<sf::Vector2f> patrolWaypoints;
};