#include "DropDownMenu.hpp"
#include "InputController.hpp"
#include "ScreenController.hpp"


DropDownMenu::DropDownMenu(InputListener * listener){
	
	isDown = false;
	dropSize = 0;
	hight = 20; //value retrieved from testing
	with = 200; //

	pos = sf::Vector2f{ 0, 0 };
	menuListener = listener;
	currentFont = new sf::Font;
	currentFont->loadFromFile(GlobalValues::MAIN_PATH() + "/fonts/arial.ttf");
}

DropDownMenu::~DropDownMenu(){
	delete currentFont;
}


void DropDownMenu::addToInputController(){
	InputController::getInstance().addFunctionListener(this);
}

void DropDownMenu::removeFromInputController(){
	InputController::getInstance().removeFunctionListener(this);
}


void DropDownMenu::tempaddFuntion(std::pair<KeyMap::function, std::string> tempFunctionList){
	dropDownFunction * workingDropDownMenu = new dropDownFunction;

	workingDropDownMenu->func = tempFunctionList.first;

	workingDropDownMenu->shape.setSize(sf::Vector2f(with, hight));
	workingDropDownMenu->shape.setFillColor(sf::Color{ 0xcd, 0xc9, 0xc9 }); //color found online using color picker.
	workingDropDownMenu->shape.setOutlineColor(sf::Color::Black);
	workingDropDownMenu->shape.setOutlineThickness(1);

	workingDropDownMenu->text = sf::Text(tempFunctionList.second, *currentFont);
	workingDropDownMenu->text.setCharacterSize(14);
	workingDropDownMenu->text.setStyle(sf::Text::Bold);
	workingDropDownMenu->text.setColor(sf::Color::Blue);

	dropDownMenuFunctions.push_back(*workingDropDownMenu);
	delete(workingDropDownMenu);

}

void DropDownMenu::addFunctionList(std::vector<std::pair<KeyMap::function, std::string>> funclist){
	dropDownMenuFunctions.clear();
	for (std::vector<std::pair<KeyMap::function, std::string>>::iterator it = funclist.begin();
		it != funclist.end(); ++it) {
		DropDownMenu::tempaddFuntion(*it);
	}
}

void DropDownMenu::show(){
	isDown = true;
	pos.x = (float)InputController::getInstance().getMousePos().x;
	pos.y = (float)InputController::getInstance().getMousePos().y;
	int i = 0;
	for (std::vector<dropDownFunction>::iterator it = dropDownMenuFunctions.begin();
		it != dropDownMenuFunctions.end(); ++it, i++) {
		it->shape.setPosition(pos.x, pos.y + (hight*i));
		it->text.setPosition(sf::Vector2f{ pos.x + 5, pos.y + (hight*i) - 1 });
	}
}


bool DropDownMenu::hoverOverDropDownMenu(){
	float mouseX = (float)InputController::getInstance().getMousePos().x;
	float mouseY = (float)InputController::getInstance().getMousePos().y;
	if ((pos.x<mouseX && (pos.x + with)>mouseX) && (pos.y<mouseY && pos.y + 
			(hight*(dropDownMenuFunctions.size()))>mouseY)){
		return true;
	}
	return false;
}
int	DropDownMenu::HoverOverButton(){
	float mouseX = (float)InputController::getInstance().getMousePos().x;
	float mouseY = (float)InputController::getInstance().getMousePos().y;
	unsigned int i;
	for (i = 0; i <= dropDownMenuFunctions.size(); i++){
		if (pos.y<mouseY && pos.y + (hight*(i))>mouseY){
			return (i - 1);
		}
	}
	return i - 1;

}

void DropDownMenu::keyPressed(KeyMap::function func){
	if (func == KeyMap::options&&!isDown){
		pos = (sf::Vector2f)InputController::getInstance().getMousePos();
		menuListener->keyPressed(KeyMap::function::setDropDownMenu);
		show();
	}
	else if (isDown&&func == KeyMap::select&&hoverOverDropDownMenu()){
		menuListener->keyPressed(dropDownMenuFunctions[HoverOverButton()].func);
		isDown = false;
	}
	else {
		isDown = false;
	}
}

std::vector<KeyMap::function> DropDownMenu::getKeyMap(){
	return std::vector<KeyMap::function>{ KeyMap::select, KeyMap::options };
}

sf::Vector2f DropDownMenu::getDropDownMouseBeginPosition(){
	return pos;
}

void DropDownMenu::draw(sf::RenderWindow & window){
	if (isDown){

		bool hover = hoverOverDropDownMenu();
		int button;
		if (hover){
			button = HoverOverButton();
			dropDownMenuFunctions[button].shape.setFillColor(sf::Color{ 0x8b, 0x89, 0x89 }); //hover color
		}

		int i=0;
		for (std::vector<dropDownFunction>::iterator it = dropDownMenuFunctions.begin();
				it != dropDownMenuFunctions.end(); ++it, i++) {
			window.draw(dropDownMenuFunctions[i].shape);
			window.draw(dropDownMenuFunctions[i].text);
		}
		if (hover){
			dropDownMenuFunctions[button].shape.setFillColor(sf::Color{ 0xcd, 0xc9, 0xc9 }); //unhover color
		}
	}
}
