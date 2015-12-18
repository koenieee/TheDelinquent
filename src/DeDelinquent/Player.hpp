#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "IDrawableComponent.hpp"
#include "UpdateableComponent.hpp"
#include "InputController.hpp"
#include "KeyMap.hpp"
#include "Movable.hpp"
#include "UnpenetrableComponent.hpp"
#include "Level.hpp"
#include "GlobalValues.hpp"
#include <memory>

/**
* Player class is used to move, draw and play with the protagonist. 
* @version 3.0
*/
class Player : public Movable, public UpdateableComponent{
public:

	/**
	* Player can be initialized using a start position
	* @param playerStartPosition The start position of the player item.
	*/
	Player::Player(sf::Vector2f playerStartPosition);

	/**
	* Destructor for player so all pointers and local objects are removed.                                                                     
	*/
	Player::~Player();

	/**
	* This will return the current player position. 
	* @return The current player position.
	*/
	sf::Vector2f Player::getPos();


	/**
	* Update the Player
	*
	* This is responsible for looking for pressed keys
	* and moving the player accordingly
	*/
	void UpdateableComponent::update() override;

	/**
	* Get the sound level the Player produces
	* @return A float containing the current sound value of player.
	*/
	float Player::getSound();

	/**
	* Add an inpenetrable component to player, so when player hits a guard, exitpoint or pickup there is interaction with player and the item.
	* @param interactable The interactable item the player has interaction with.
	*/
	void Player::addInteractable(std::shared_ptr<UnpenetrableComponent> interactable);

	/**
	* This function sets the awareness level of player to the max, when player is spotted by the guards.                                                                     
	*/
	void Player::hasBeenSpotted();
	
	/**
	* Player can clear all the interactables connected to the player.                                                                     
	*/
	void Player::clearInteractables();

private:
	sf::Image temp;
	sf::Texture playerImage;
	sf::Sprite loadedPlayerImage;
	float playerSoundMultiplier = 0;

	std::vector<std::shared_ptr<UnpenetrableComponent>> allInteractables;

	const std::string personImage = GlobalValues::MAIN_PATH() + "images/protagonistTextureAll.png";

	//tweakable values
	const float awarenessMultuplier = 0.58f;
	const int ratioBetweenSoundAndStanding = 2;
	const float inscreaseSpeedOfSound = 0.04f;
	const float runningSound = 0.75f;
	const float sneakingSound = 0.06f;
	const float walkingSound = 0.1f;
	const float stealthAbilityMultiplier = 0.2f;
	const float baseSpeed = 1.4f;

	//player specific values.
	float sound = 0;
	float awareness = 0;
	float stealthAbility;

	void checkInteraction();

};
