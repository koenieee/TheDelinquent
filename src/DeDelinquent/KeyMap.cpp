#include "KeyMap.hpp"

KeyMap::KeyMap(){

	for (int i = 0; i < KeyMap::count; i++)
	{
		keyBoundList[i] = (sf::Keyboard::Key)0xFF; // Initialize with non-functional key
	}

	KeyMap::keyBoundList[KeyMap::moveUp] = sf::Keyboard::W;
	KeyMap::keyBoundList[KeyMap::moveDown] = sf::Keyboard::S;
	KeyMap::keyBoundList[KeyMap::moveLeft] = sf::Keyboard::A;
	KeyMap::keyBoundList[KeyMap::moveRight] = sf::Keyboard::D;
	KeyMap::keyBoundList[KeyMap::arrowUp] = sf::Keyboard::Up;
	KeyMap::keyBoundList[KeyMap::arrowDown] = sf::Keyboard::Down;
	KeyMap::keyBoundList[KeyMap::arrowLeft] = sf::Keyboard::Left;
	KeyMap::keyBoundList[KeyMap::arrowRight] = sf::Keyboard::Right;
	KeyMap::keyBoundList[KeyMap::pause] = sf::Keyboard::P;
	KeyMap::keyBoundList[KeyMap::use] = sf::Keyboard::E;
	KeyMap::keyBoundList[KeyMap::exit] = sf::Keyboard::Escape;
	KeyMap::keyBoundList[KeyMap::run] = sf::Keyboard::LShift;
	KeyMap::keyBoundList[KeyMap::sneak] = sf::Keyboard::LControl;
	for (int i = 0; i < KeyMap::count; i++)
	{
		mouseBoundList[i] = (sf::Mouse::Button)0xFF; // Initialize with non-functional value
	}
	KeyMap::mouseBoundList[KeyMap::select] = sf::Mouse::Left;
	KeyMap::mouseBoundList[KeyMap::options] = sf::Mouse::Right;
	KeyMap::mouseBoundList[KeyMap::middle] = sf::Mouse::Middle;
}

