#include "SoundBar.hpp"


SoundBar::SoundBar(){}

void SoundBar::setWindow(){
	sf::Vector2f windowSize = sf::Vector2f{ (float)GlobalValues::WINDOW_SIZE_X, (float)GlobalValues::WINDOW_SIZE_Y };
	SoundBar::soundBar.setPosition(sf::Vector2f{ windowSize.x*0.73f, windowSize.y*0.83f });
	SoundBar::soundBar.setOutlineColor(sf::Color::Green);
	SoundBar::soundBar.setOutlineThickness(3.0);
	SoundBar::soundBar.setSize(sf::Vector2f{ windowSize.x*0.15f, windowSize.y*0.12f });
	SoundBar::soundBar.setFillColor(sf::Color::Transparent);

	//initialize middle line
	SoundBar::baseLine.setPosition(sf::Vector2f{ windowSize.x*0.73f, windowSize.y*0.89f });
	SoundBar::baseLine.setOutlineColor(sf::Color::Black);
	SoundBar::baseLine.setOutlineThickness(0.5);
	SoundBar::baseLine.setSize(sf::Vector2f{ windowSize.x*0.15f, 0 });

	
	SoundBar::line.setFillColor(sf::Color::Cyan);
	

}
void SoundBar::drawSoundBar(sf::RenderWindow &window){
	window.draw(soundBar);
	window.draw(baseLine);
	window.draw(line);
}

void SoundBar::updateSound(){
	float temp = 0.04f*soundMultiplier;
	sf::Vector2f windowSize = sf::Vector2f{ (float)GlobalValues::WINDOW_SIZE_X, (float)GlobalValues::WINDOW_SIZE_Y };
	SoundBar::line.setPosition(sf::Vector2f{ windowSize.x*0.73f, windowSize.y*(0.89f-temp) });
	SoundBar::line.setSize(sf::Vector2f{ windowSize.x*baseWidthSound*soundMultiplier, windowSize.y*baseLenghtSound*soundMultiplier });
	
}

void SoundBar::setSoundMultiplier(float sm){
	if (sm>=0 && sm<=1.5){
		soundMultiplier = sm;
	}
}

float SoundBar::getSoundMultiplier(){
	return soundMultiplier;
}

