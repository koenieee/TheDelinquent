#include "Player.hpp"
#include <iostream>
#include "PlayerData.hpp"
#include "ScreenController.hpp"
#include "SoundBar.hpp"
#include "ArtificialIntelligence.hpp"
#include "AwarenessMeter.hpp"


Player::Player(sf::Vector2f playerStartPosition){
	IDrawableComponent::pos = playerStartPosition;
	Movable::setDrawing(personImage);
	setSpeed(baseSpeed + (PlayerData::getInstance().getPlayerSpeed())*0.1); //values found doing playtest.
	stealthAbility = (1.0f - (stealthAbilityMultiplier*((float)PlayerData::getInstance().getStealthAbility() - 1.0f)));
}

Player::~Player() {
	for (auto & i : allInteractables) {
		i = nullptr;
	}
	allInteractables.clear();
}

sf::Vector2f Player::getPos(){
	return pos;
}

void Player::hasBeenSpotted()
{
	//increase awareness to maximum
	awareness = 255;
}

void Player::update(){
	checkInteraction();

	//setSpeed(1 + (PlayerData::getInstance().getPlayerSpeed())*0.1);

	sf::Vector2f move = { 0, 0 };
	if (InputController::getInstance().getfunction(KeyMap::moveUp)){ move.y--; }
	if (InputController::getInstance().getfunction(KeyMap::moveDown)){ move.y++; }
	if (InputController::getInstance().getfunction(KeyMap::moveLeft)){ move.x--; }
	if (InputController::getInstance().getfunction(KeyMap::moveRight)){ move.x++; }
	

	if (InputController::getInstance().getfunction(KeyMap::run)){ 
		setRunning(true);}//{ move = move + move; }
	else{ setRunning(false); }
	if (InputController::getInstance().getfunction(KeyMap::sneak)){ setSneaking(true); }//{ move.x = move.x / 2;move.y=move.y/2; }
	else { setSneaking(false); }

	//moves the object (relative to the speed...)
	Movable::move(move);//returns an bool if it was possible to move to that pos..
	ScreenController::centerView((Movable::theMovingImage.getPosition() + sf::Vector2f{21.0f, 21.0f}));



	//update sound
	if (move != sf::Vector2f{ 0, 0 }){
		if (isRunning()){ 
			if (sound < runningSound){
				sound = sound + inscreaseSpeedOfSound;
			}
			else if (sound>runningSound + inscreaseSpeedOfSound){
				sound = sound-inscreaseSpeedOfSound;
			}
		}
		else if (isSneaking()){
			if (sound < sneakingSound){
				sound = sound+inscreaseSpeedOfSound;
			}
			else if (sound>sneakingSound + inscreaseSpeedOfSound){
				sound = sound - inscreaseSpeedOfSound;
			}
		}
		else{
			if (sound < walkingSound){
				sound = sound+inscreaseSpeedOfSound;
			}
			else if (sound>walkingSound + inscreaseSpeedOfSound){
				sound = sound - inscreaseSpeedOfSound;
			}
		}
	}
	else { 
		if (sound <= inscreaseSpeedOfSound){
			sound = 0.0f;
		}
		else if (sound>inscreaseSpeedOfSound){ sound = sound - inscreaseSpeedOfSound; }
		
	}
	SoundBar::getInstance().setSoundMultiplier(sound * 2);


	//update awareness

	//awareness = sound* distance of closest guard
	float temp = 0;
	float temp1 = ArtificialIntelligence::getSmallestDistance();

	if (temp1 > 500||temp1<1){ temp = 0; }
	else{ temp = (500 - temp1)*awarenessMultuplier; }


	awareness = (((stealthAbility*sound)* temp*ratioBetweenSoundAndStanding) + temp) / (ratioBetweenSoundAndStanding + 1);

	if (awareness > 254){ awareness = 255; }

	AwarenessMeter::getInstance().awarenessUpdate(awareness);


	if (awareness>180){ //level 3 awareness
		ArtificialIntelligence::setAwarenessLevel(3); 
	}
	else if (awareness>90){ //level 2 awareness
		ArtificialIntelligence::setAwarenessLevel(2);
	}
	else{
		ArtificialIntelligence::setAwarenessLevel(1); 
	}

} 

float Player::getSound(){
	playerSoundMultiplier = 1;
	if (isRunning()){
		playerSoundMultiplier = 1.5;
	}
	else if (isSneaking()){
		playerSoundMultiplier = 0.5;
	}
	return playerSoundMultiplier;
}

void Player::checkInteraction(){
	for (auto & interactable : Player::allInteractables){
		if (interactable->overlappingPosition(sf::FloatRect{ Movable::theMovingImage.getPosition(), sf::Vector2f{ 40.0f, 40.0f } })){
			
		}
	}
}

void Player::addInteractable(std::shared_ptr<UnpenetrableComponent> interactable){
	allInteractables.push_back(interactable);
}

void Player::clearInteractables(){
	for(auto & i : allInteractables) {
		i = nullptr;
	}
	allInteractables.clear();
}
