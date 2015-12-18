#include "Movable.hpp"
#include "ScreenController.hpp"

UnpenetrableComponent * Movable::theLevel;

Movable::~Movable() {

}

void Movable::setSpeed(float newSpeed){
	speed = newSpeed;
}

void Movable::draw(sf::RenderWindow &window){
	Movable::theMovingImage.setTextureRect(sf::IntRect{ 43 * Movable::orientation, 0, 43, 43 }); //size of picture of player or guard

	window.draw(Movable::theMovingImage);
}

void Movable::setDrawingPosition(sf::Vector2f newPos){
	Movable::theMovingImage.setPosition(newPos);
}


void Movable::setTheLevel(UnpenetrableComponent * theLevel){
	Movable::theLevel = theLevel; //only level, so no other componenents.
}

bool Movable::move(sf::Vector2f moveTowards){

	//changes orientation of the movable
	setOrientation(moveTowards);
	
	//canMove(Movable::theMovingImage.getPosition(), &moveTowards, isRunning(), isSneaking());
	
	if (!canMove(Movable::theMovingImage.getPosition(), &moveTowards, isRunning(), isSneaking())){
		pos += moveTowards;
		setDrawingPosition(pos);
		return false;
	}
	else{
		pos += moveTowards;
		setDrawingPosition(pos);
		return true;
	}
	
	//pos += moveTowards;
	//setDrawingPosition(pos);
	//return moveTowards.x != 0 || moveTowards.y != 0;
}

bool Movable::canMove(sf::Vector2f position,sf::Vector2f * moveTowards,bool isRunning,bool isSneaking){
	bool problem1 = false;
	bool problem2 = false;
	//diagonal you will have the same speed.
	
	if (abs(moveTowards->x) == abs(moveTowards->y)){
		*moveTowards *= diagonalSpeedMultiplier;
	}
	*moveTowards = speed*(*moveTowards);
	if (isRunning){
		*moveTowards *= runningSpeedMultiplier;
	}
	else if (isSneaking) {
		*moveTowards *= sneakingSpeedMultiplier;
	}

	if (Movable::theLevel->overlappingPosition(sf::FloatRect{ position + sf::Vector2f{ moveTowards->x, 0 }, sf::Vector2f{ 40.0f, 40.0f } })){
		moveTowards->x = 0;
		problem1 = true;
	}
	if (Movable::theLevel->overlappingPosition(sf::FloatRect{ position + sf::Vector2f{ 0, moveTowards->y }, sf::Vector2f{ 40.0f, 40.0f } })){
		moveTowards->y = 0;
		problem2 = true;
	}

	//bool shouldreturn = (problem1 || problem2);
	return !(problem1 || problem2);
}


void Movable::setRunning(bool run){ 
	if (running != run){
		running = run;
	}
}

bool Movable::isRunning(){
	if (running&&!sneaking){ return true; }
	return false;
}

void Movable::setSneaking(bool sneak){ 
	if (sneaking != sneak){
		sneaking = sneak;
	}
}

bool Movable::isSneaking(){
	if (sneaking&&!running){ return true; }
	return false;
}


void Movable::setOrientation(sf::Vector2f newOrientation){
	if (newOrientation != sf::Vector2f{ 0, 0 }){
		sf::Vector2i temp = (sf::Vector2i)newOrientation + sf::Vector2i{ 1, 1 };
		if (temp.x == 0){
			//west
			if (temp.y == 0){		//north-west
				orientation = 7;
			}
			else if (temp.y == 1){	//west
				orientation = 6;
			}
			else if (temp.y == 2){	//south-west
				orientation = 5;
			}
		}
		else if (temp.x == 1){
			//neutral
			if (temp.y == 0){		//north
				orientation = 0;
			}
			else if (temp.y == 2){	//south
				orientation = 4;
			}
		}
		else if (temp.x == 2){
			//east
			if (temp.y == 0){		//north-east
				orientation = 1;
			}
			else if (temp.y == 1){	//east
				orientation = 2;
			}
			else if (temp.y == 2){	//south-east
				orientation = 3;
			}
		}
	}
}

void Movable::setDrawing(const std::string & filename){
	if (!Movable::temp.loadFromFile(filename)){
		throw new std::exception("PLAYER IMAGE NOT FOUND");
	}
	Movable::loadedImage.loadFromImage(temp);
	Movable::loadedImage.setSmooth(true);
	Movable::theMovingImage.setTexture(Movable::loadedImage, true);
	Movable::theMovingImage.setTextureRect(sf::IntRect{ 0, 0, 43, 43 }); //texture rect is size of guard or player image.
	Movable::theMovingImage.setPosition(pos);
	Movable::theMovingImage.setOrigin(3, 3); //Origin location
	Movable::theMovingImage.setScale(sf::Vector2f{ 1.14f, 1.14f }); //values found by trail and error
}

sf::FloatRect & Movable::getHitbox(){ return theMovingImage.getLocalBounds(); };
