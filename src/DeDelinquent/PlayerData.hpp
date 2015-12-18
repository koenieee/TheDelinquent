#pragma once

#include <SFML/Graphics.hpp>
#include "rapidjson/document.h"
#include "DiskController.hpp"
#include "GlobalValues.hpp"
#include <iostream>
#include <fstream>
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include <string>
#include <string.h>

/**
* Class for Player Data
* @version 1.2
*/



class PlayerData
{
private:
	
	PlayerData();

	~PlayerData();


public:

	/**
	* Get the PlayerData instance.
	* 
	* @return the PlayerData reference.
	*/
	static PlayerData& getInstance(){

		static PlayerData thisInstance;
		return thisInstance;
	}

	/**
	* Saves the starting position of the Player.
	*
	* @param drawPos a sf::Vector2f value.
	*/
	void setDrawPos(sf::Vector2f drawPos);

	/**
	* Set the level count of the Player.
	*
	* @param lvlnm a integer value.
	*/
	void setLevelNumber(int lvlnm);

	/**
	* Get the level count of the Player.
	*
	* @return a int the current level.
	*/
	int getLevelNumber();


	/**
	* Updates the Experience Points.
	*
	* Skillpoints and Experience points are needed for next skill points.
	* @param exp an integer value.
	*/
	void updateExperience(int exp);

	/**
	* Give experience points for the Main Objective.
	* 
	* @param exp an int value.
	*/
	void mainObjectiveExp(int exp);
	
	/**
	* Get experience points value for the Main Objective.
	*
	* @return an int value.
	*/
	int getMainObjectiveExp();

	/**
	* Get the amount of skill points the player can spend.
	*/
	int getSkillPoints();

	/**
	* Increase the speed multiplier of the Player.
	* 
	* @param ps an integer value.
	*/
	void increasePlayerSpeed(int ps);

	/**
	* Get the speed multiplier needed for the level to calculate the speed of the player.
	*
	* @return an int value, Player movement speed.
	*/
	int getPlayerSpeed();

	/**
	* Increase the current stealth ability of the Player.
	* 
	* @param sa an integer value.
	*/
	void increaseStealthAbility(int sa);

	/**
	* Get the current stealth ability of the Player.
	*
	* @return an int value.
	*/
	int getStealthAbility();


	/**
	* Increase the awereness skill of the Player.
	* 
	* @param am an integer value.
	*/
	void increaseAwarenessSkill(int am);
	
	/**
	* Get the awereness skill of the Player.
	*
	* @return an int value.
	*/
	int getAwarenessSkill();

	/**
	* Drawing Implementation.
	*
	* @param window a sf::RenderWindow reference.
	*/
	void draw(sf::RenderWindow & window);
	
	/**
	* Load the player data from a save file.
	*/
	void loadPlayerDataFromFile();

	/**
	* Save the Player data to a save file.
	*/
	void savePlayerDataToFile();

	/**
	* This will remove the PlayerSaveFile so all the values are back to the default values specified in PlayerData.hpp                                                                     
	*/
	void resetPlayerDataFile();
	
	/**
	* Set Exit points active.
	*
	* @param canfinish, a bool value.
	*/
	void playerMayFinish(bool canfinish);

	/**
	* Get if player may finish.
	* 
	* @return a bool value.
	*/
	bool playerCanFinish();

	

private:

	std::vector<std::string> textKeyNames = std::vector<std::string>{ "PL_DATA","EXP_POINTS","LVL_NUM","P_SPEED","A_SKILL"};
	std::string PlayerDataFilename = std::string{ GlobalValues::MAIN_PATH() + "json_data/PlayerData_nl.json" };
	
	std::vector<std::string> playerDataVariables = std::vector < std::string > {"LEVEL_NUMBER", "SKILL_POINTS", "PLAYER_SPEED", "STEALTH_ABILITY", "AWERENESS_SKILL", "XP_NEEDED_FOR_SKILL_POINT", "XP_GATHERED"};
	std::string playerDataSaveFile = std::string{ GlobalValues::MAIN_PATH() + "json_data/PlayerSaveFile.json" };
	std::map<std::string, std::string *> * buttons;

	//private method
	sf::Text * PlayerData::genText(std::string theName, sf::Vector2f pos, int TextSize, sf::Color theCol);

	void clearAllTextPointers();

	bool canPlayerFinish = false;

	//only called by all the set() functions
	void reGenerateSFMLtext();

	std::vector<sf::Text *> playerTextSFML;
	sf::Vector2f drawMeAtPos;

	int levelNumber = 1;
	
	//start values
	int skillPoints = 0;
	int playerSpeed = 1; 
	int stealthAbility = 1;
	int awarenessSkill = 50; // still need to be implemented
	
	//base value for the first skill point
	int baseValueExperience = 1000;
	int experienceNeededForSkillPoint = 1000;
	int experienceGathered = 0;
	
	int almostExperience = 0;


	sf::Font theUsedFont;

	bool generateMe = false;
};