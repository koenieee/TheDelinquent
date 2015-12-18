#include "AwarenessMeter.hpp"

//TODO: a lot of magic numbers

AwarenessMeter::AwarenessMeter()
{
	sf::Vector2f windowSize = sf::Vector2f{1024, 768};

	AwarenessMeter::border.setFillColor(sf::Color{ 255, 0, 0, 0 });
	AwarenessMeter::border.setOutlineColor(sf::Color{ 128, 128, 128, 255 });
	AwarenessMeter::border.setOutlineThickness(3);
	AwarenessMeter::border.setPosition(sf::Vector2f{ windowSize.x*0.9f, windowSize.y*0.75f });
	AwarenessMeter::border.setSize(sf::Vector2f{ windowSize.x*0.06f, windowSize.y*0.2f });
	
	AwarenessMeter::divider1.setPosition(sf::Vector2f{ windowSize.x*0.9f, windowSize.y*(0.75f + 0.13333f) });
	AwarenessMeter::divider1.setSize(sf::Vector2f{ windowSize.x*0.06f, 3.0f });
	AwarenessMeter::divider1.setFillColor(sf::Color{ 128, 128, 128, 255 });

	AwarenessMeter::divider2.setPosition(sf::Vector2f{ windowSize.x*0.9f, windowSize.y*(0.75f + 0.06666f) });
	AwarenessMeter::divider2.setSize(sf::Vector2f{ windowSize.x*0.06f, 3.0f });
	AwarenessMeter::divider2.setFillColor(sf::Color{ 128, 128, 128, 255 });

	AwarenessMeter::AwarenessBar.setPosition(sf::Vector2f{ windowSize.x*0.9f, windowSize.y*(0.75f + 0.2f)});
	AwarenessMeter::AwarenessBar.setSize(sf::Vector2f{ windowSize.x*0.06f, windowSize.y*(0.0f) });

	sf::Uint8 redness = 255;
	sf::Uint8 greenness = 0;
	AwarenessMeter::AwarenessBar.setFillColor(sf::Color{ redness, greenness, 0, 255 });
}


void AwarenessMeter::windowUpdate(sf::RenderWindow &window){
	window.draw(AwarenessMeter::AwarenessBar);
	window.draw(AwarenessMeter::divider1);
	window.draw(AwarenessMeter::divider2);
	window.draw(AwarenessMeter::border);
}


void AwarenessMeter::awarenessUpdate(int raw){	//ranging from 0 to 255
	sf::Vector2f windowSize = sf::Vector2f{1024, 768};

	AwarenessMeter::AwarenessBar.setPosition(sf::Vector2f{ windowSize.x*0.9f, windowSize.y*(0.75f + ((255-raw)*sizerFactor) )});
	AwarenessMeter::AwarenessBar.setSize(sf::Vector2f{ windowSize.x*0.06f, windowSize.y*(raw*sizerFactor) });

	sf::Uint8 redness = raw;
	sf::Uint8 greenness = 255 - raw;
	AwarenessMeter::AwarenessBar.setFillColor(sf::Color{ redness, greenness, bluenessFactor, 255 });
}