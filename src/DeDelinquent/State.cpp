#include "State.hpp"
#include "InputController.hpp"

const std::string State::imageFileName = GlobalValues::MAIN_PATH() + "images/HayFase_SplashScreen.jpg";
sf::Image State::temp;
sf::Texture State::background;
sf::Sprite State::loadedBackground;

void State::drawScrollable(sf::RenderWindow & window){

}
void State::drawFixed(sf::RenderWindow & window){

}
void State::update(){

}

void State::mouseHover(sf::Vector2i position){

}

State::~State(){
// TODO: add Destructor logic
}


std::pair<sf::Text *, sf::RectangleShape *>  * State::combineButtonAndText (std::string Text, sf::Vector2f position, int screenWidth){
	sf::Vector2f buttonPos = position;
	int text_length = Text.size();
	sf::Text * thet = genText(Text, sf::Vector2f(((float)(screenWidth / 2) - text_length * 5), position.y)); //generate text use as width the screen width / 2 - Text length * 5== 5 is found by trail and error.
	int size = thet->getString().getSize();


	sf::RectangleShape * rcc = drawRect(buttonPos, sf::Color{ 64, 220, 225 }, sf::Vector2f{ (float)(screenWidth), (float)(thet->getGlobalBounds().height + 10) }); //Color value is generated using online tool. +10 value is found by trail and error.
	std::pair<sf::Text  *, sf::RectangleShape *>  * textAndRectangle = new std::pair< sf::Text  *, sf::RectangleShape * >;
	textAndRectangle->first = thet;
	textAndRectangle->second = rcc;
	return textAndRectangle;
}

sf::Font * State::createFont(){
	if (m_font == nullptr){
		m_font = new sf::Font;
		m_font->loadFromFile(GlobalValues::MAIN_PATH() + "fonts/PRISTINA.TTF"); //load font, but only when it's not yet loaded.
		return m_font;
	}
	else{
		return m_font;
	}
}


// Memory leak?
sf::Text * State::genText(std::string theName, sf::Vector2f pos, int TextSize, sf::Color theCol){
	sf::Text * text = new sf::Text(theName, *createFont());
	text->setCharacterSize(TextSize);
	text->setPosition(pos);
	
	//text.setStyle(sf::Text::Bold);
	text->setColor(theCol);
	return text;

}

sf::RectangleShape * State::drawRect(sf::Vector2f position, sf::Color theColor,	sf::Vector2f sizeRectangle){
	sf::RectangleShape * rectShape = new sf::RectangleShape;
	rectShape->setSize(sizeRectangle);
	rectShape->setPosition(position);
	rectShape->setFillColor(sf::Color::Transparent);
	return rectShape;
}

void State::drawBackground(sf::RenderWindow & window){
	window.draw(State::loadedBackground);
}

void State::loadBackground(){
	if (!State::temp.loadFromFile(imageFileName)){
		throw new std::exception("SPLASH IMAGE NOT FOUND");
	}

	State::background.loadFromImage(temp);
	State::loadedBackground.setTexture(background, true);
	State::loadedBackground.setScale(sf::Vector2f{1.0f, 1.0f });
	State::loadedBackground.setPosition(sf::Vector2f{ 0, 0 });
}