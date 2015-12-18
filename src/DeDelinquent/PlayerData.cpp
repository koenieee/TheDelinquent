#include "PlayerData.hpp"
#include <iostream>
#include "GlobalValues.hpp"


PlayerData::PlayerData() 
{
	rapidjson::Document * thed = DiskController::readFileIntoJson(PlayerDataFilename);
	buttons = DiskController::returnJsonObjects(*thed, textKeyNames);
	theUsedFont = sf::Font();
	theUsedFont.loadFromFile(GlobalValues::MAIN_PATH() + "fonts/PRISTINA.TTF");
	std::ifstream checkSaveFile(playerDataSaveFile);
	if (checkSaveFile.good()){
		PlayerData::getInstance().loadPlayerDataFromFile();
	}
}

void PlayerData::setDrawPos(sf::Vector2f drawPos){
	drawMeAtPos = drawPos;
	reGenerateSFMLtext();
}

void PlayerData::clearAllTextPointers(){
	for (sf::Text * cT : playerTextSFML){
		delete cT;
	}
}

PlayerData::~PlayerData(){
}

void PlayerData::resetPlayerDataFile(){
	
	std::remove(playerDataSaveFile.c_str());
	//PlayerData::getInstance().~PlayerData();
}

void PlayerData::playerMayFinish(bool canfinish){
	canPlayerFinish = canfinish;
}

bool PlayerData::playerCanFinish(){
	return canPlayerFinish;
}

void PlayerData::mainObjectiveExp(int exp){
	
	almostExperience = exp;
}

int PlayerData::getMainObjectiveExp(){
	return almostExperience;
}

void PlayerData::reGenerateSFMLtext(){
	clearAllTextPointers();

	playerTextSFML.clear(); //clearing init vector


	playerTextSFML.push_back(genText(buttons->at("PL_DATA")->c_str(), sf::Vector2f{
		drawMeAtPos.x, drawMeAtPos.y }, 30, sf::Color{ 0, 255, 251 }));

	if (getSkillPoints() == 0){
		playerTextSFML.push_back(genText(buttons->at("EXP_POINTS")->c_str() + 
			std::to_string(getSkillPoints()), sf::Vector2f{ drawMeAtPos.x,
			drawMeAtPos.y + 50 }, 30, sf::Color::Red));
	}
	else{
		playerTextSFML.push_back(genText(buttons->at("EXP_POINTS")->c_str() + 
			std::to_string(getSkillPoints()), 
			sf::Vector2f{ drawMeAtPos.x, drawMeAtPos.y + 50 }, 30, sf::Color{ 0, 255, 251 }));
	}


	playerTextSFML.push_back(genText(buttons->at("LVL_NUM")->c_str() +
		std::to_string(getLevelNumber()), sf::Vector2f{ drawMeAtPos.x,
		drawMeAtPos.y + 100 }, 30, sf::Color{ 0, 255, 251 }));

	playerTextSFML.push_back(genText(buttons->at("P_SPEED")->c_str() + 
		std::to_string(getPlayerSpeed()), sf::Vector2f{ drawMeAtPos.x, 
		drawMeAtPos.y + 200 }, 30, sf::Color{ 0, 255, 251 }));

	playerTextSFML.push_back(genText(buttons->at("A_SKILL")->c_str() + 
		std::to_string(getStealthAbility()), sf::Vector2f{ drawMeAtPos.x, 
		drawMeAtPos.y + 300 }, 30, sf::Color{ 0, 255, 251 }));
	generateMe = false;
}

void PlayerData::draw(sf::RenderWindow & window){
	if (generateMe){
		reGenerateSFMLtext();
	}
	for (sf::Text * cT : playerTextSFML){
		window.draw(*cT);
	}
}

sf::Text * PlayerData::genText(std::string theName, sf::Vector2f pos, 
	int TextSize, sf::Color theCol){
	sf::Text * text = new sf::Text(theName, theUsedFont);
	text->setCharacterSize(TextSize);
	text->setPosition(pos);
	text->setColor(theCol);
	return text;

}

void PlayerData::setLevelNumber(int lvlnm){
	levelNumber = lvlnm;
	generateMe = true;
}
int PlayerData::getLevelNumber(){
	return levelNumber;
}

void PlayerData::updateExperience(int exp){
	experienceGathered += exp;
	while (experienceNeededForSkillPoint <= experienceGathered){
		experienceNeededForSkillPoint = baseValueExperience + experienceNeededForSkillPoint*1.1;
		skillPoints += 1;
	}
	generateMe = true;
}
int PlayerData::getSkillPoints(){
	return skillPoints;
}


void PlayerData::increasePlayerSpeed(int ps){
	if ((skillPoints - ps) >= 0){
		skillPoints -= ps;
		playerSpeed += ps;
		generateMe = true;
	}

	
}
int PlayerData::getPlayerSpeed(){
	return playerSpeed; 
}

void PlayerData::increaseAwarenessSkill(int am){
	if ((skillPoints - am) >= 0){
		skillPoints -= am;
		playerSpeed += am;
		generateMe = true;
	}


}
int PlayerData::getAwarenessSkill(){
	return PlayerData::awarenessSkill;
}

void PlayerData::increaseStealthAbility(int sa){
	if ((skillPoints - sa) >= 0){
		skillPoints -= sa;
		stealthAbility += sa;
		generateMe = true;
	}
}
int PlayerData::getStealthAbility(){
	return stealthAbility;
}

void PlayerData::loadPlayerDataFromFile(){
	rapidjson::Document & pdsf = *DiskController::readFileIntoJson(playerDataSaveFile);

	levelNumber = pdsf["LEVEL_NUMBER"].GetInt();
	skillPoints = pdsf["SKILL_POINTS"].GetInt();
	playerSpeed = pdsf["PLAYER_SPEED"].GetInt();
	stealthAbility = pdsf["STEALTH_ABILITY"].GetInt();
	awarenessSkill = pdsf["AWERENESS_SKILL"].GetInt();
	experienceNeededForSkillPoint = pdsf["XP_NEEDED_FOR_SKILL_POINT"].GetInt();
	experienceGathered = pdsf["XP_GATHERED"].GetInt();
	
}

void PlayerData::savePlayerDataToFile(){

	std::vector<int> allSaveData = std::vector < int > {levelNumber, skillPoints, playerSpeed, stealthAbility, awarenessSkill, experienceNeededForSkillPoint, experienceGathered};

	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

	int iterator = 0;
	for (std::string pld : playerDataVariables){
		rapidjson::Value valLevelNumberObject(rapidjson::kNumberType);
		int currentSave = allSaveData.at(iterator);
		valLevelNumberObject.SetInt(currentSave);


		rapidjson::Value & playerStringName = *new rapidjson::Value(rapidjson::kStringType);
		std::string & name = *new std::string{pld}; //this is just weird, but yeah it works. I could also just use a pointer, but now we don't have to use *
		playerStringName.SetString(name.c_str(), name.size());


		jsonDoc.AddMember(playerStringName, valLevelNumberObject, jsonDoc.GetAllocator());
		iterator++;
	}



	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();  // to examine the encoding...

	std::ofstream out(playerDataSaveFile);
	out << jsonString;
	out.close();

}

