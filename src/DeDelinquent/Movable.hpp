#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "IDrawableComponent.hpp"
#include "Level.hpp"
#include "UnpenetrableComponent.hpp"

/**
* Provides methods for an Movable component
* @version 1.1
*/
class Movable :public IDrawableComponent{
public:

	/**
	* Destroys the Movable instance
	*/
	virtual Movable::~Movable();

	/**
	Sets the speed and calculates diagonal speed of the Player
	@param newSpeed The speed that will be used when the players moves
	*/
	virtual void Movable::setSpeed(float newSpeed);

	/**
	* This will return the local bounds of the currently loaded texture Image.                                                              
	* @return The LocalBounds of the currently loaded image.
	*/
	virtual sf::FloatRect & Movable::getHitbox();


	/*
	* This will draw the current loaded sprite and image on the screen.
	*/
	void IDrawableComponent::draw(sf::RenderWindow &window) override;


	/**
	* Static method to set the Level for collision detection, because there are multiple movables this method is only one time called for all the movables.                                                                     
	*/
	static void Movable::setTheLevel(UnpenetrableComponent * theLevel);

	
private:
	void setOrientation(sf::Vector2f moveTowards);
	bool running = false;
	bool sneaking = false;

	sf::Image temp;
	sf::Texture loadedImage;
	
	/**
	* A list of all UnpenetrableComponents the Movable Object can't walk through.                                                                     
	*
	*/
	static UnpenetrableComponent * theLevel;

protected:

	//tweakables
	float runningSpeedMultiplier = 2.0f;	//greater than 1
	float sneakingSpeedMultiplier = 0.5f;	//less then 1

	/**
	* Works as movable.setPosition(pos); give the specific movable the ability to make an compilaition of shapes&sprites.
	* @param newPos the position the shape/sprite will be drawn
	*/
	virtual void setDrawingPosition(sf::Vector2f newPos);

	/**
	* Moves to a direction multiplied with the speed (walking,running or sneaking)
	* @see orientation for an better explanation about moveTowards value.
	* @param moveTowards is an direction x&y need to be -1 , 0 or 1 (not in between)
	*/
	bool move(sf::Vector2f moveTowards);
	
	/**
	* Checks if the object can moves to a direction multiplied with the speed (walking,running or sneaking)
	* @see orientation for an better explanation about moveTowards value.
	* @param moveTowards is an direction x&y need to be -1 , 0 or 1 (not in between) 
	*/
	bool canMove(sf::Vector2f position,sf::Vector2f * moveTowards,bool isRunning,bool isSneaking);

	/**
	* Set running boolean to run and will update the speeds calculations.
	* @param run is true when you want to run, en false if you don't want it to run.
	*/
	void setRunning(bool run);

	/**
	* Determines if the player is running or not.
	* @return true if running, and false when not running(also false if running and sneaking are both true)
	*/
	bool isRunning();
	
	/**
	* Set sneaking boolean to sneak and will update the speeds calculations.
	* @param sneak is true when you want to sneak, en false if you dont want it to sneak.
	*/
	void setSneaking(bool sneak);
	
	/**
	* Determines if the player is Sneaking.
	* @return true if sneaking, and false when not sneaking(also false if running and sneaking are both true)
	*/
	bool isSneaking();

	/**
	* This function will create an image and texture based on the filename that was specified.                                                                     
	* @param filename The image filename you want to load
	*/
	void setDrawing(const std::string & filename);

	/**
	* Diagonal speed multiplier, to determine the speed when the player is walking diagonal.                                                                     
	*/
	const float diagonalSpeedMultiplier = 1/sqrt(2);


	/**
	* Used to tweak the overall speed of the Movable Object.                                                                     
	*
	*/
	float speed;

	/**
	* Used to tweak the diagonalSpeed of the Movable Object.                                                                     
	*
	*/
	float diagonalSpeed;

	/**
	orientation has the following code system:
		x	y	name		binary
	0	0	-1	north		0000
	1	1	-1	north-east	0001
	2	1	0	east		0010
	3	1	1	south-east	0011
	4	0	1	south		0100
	5	-1	1	south-west	0101
	6	-1	0	west		0110
	7	-1	-1	north-west	0111
	*/
	char orientation=0;


	/**
	* The sprite containing the texture to display.                                                                    
	*/
	sf::Sprite theMovingImage;

};