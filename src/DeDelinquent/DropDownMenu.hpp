#pragma once


#include <array>
#include <string>
#include "InputListener.hpp"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\Text.hpp>

/**
* @version 2.0
* This class  generates an dropdown menu with an maximum of 20 options.
* The class using a DropDownMenu must be an InputListener.
* This will give an KeyMap::setDropDownMenu to the listener when tried to open the dropDownMenu, this can be used to call update addFunctionList to make a specific dropDownMenu.
*/

class DropDownMenu: public InputListener{
public:

	/**
	* DropdownMenu can be created with an inputlistener so the dropdownmenu is connected to the inputlistener.
	* @param listener is an pointer to the InputListener where the KeyPressed function will be called.
	*/
	DropDownMenu(InputListener * listener);

	/**
	* Add an extra option to the drop menu, with a maximum of 20 options. If there are fewer items, the reset will be ignored.
	* @param func The function that will be given to the InputListener if the button is pressed.
	* @param name A string that will be seen in the dropDownMenu
	*/
	void addFuntion(KeyMap::function func, std::string name);

	/**
	* This is handy to ignore other keys pressed while the mouse is hovering over the dropdownmenu
	* @return true if the mouse is hovering over the dropdownmenu
	*/
	bool hoverOverDropDownMenu();

	/**
	* This will be called by the InputController. updates when select of options function is pressed.
	* @param func the function that is pressed.
	*/
	void keyPressed(KeyMap::function func) override;

	/**
	* Draws the dropdown menu when its down, it will also highlight the selection when you hover over it.
	* @param window the window the dropDownMenu will be drawn on.
	*/
	void draw(sf::RenderWindow & window);


	/**
	* The latest begin position of where the dropdown menu was when it was shown.
	* @return the position of where the menu started.
	*/
	sf::Vector2f getDropDownMouseBeginPosition();

	/**
	* Add this object as an InputListener to InputController
	*/
	void DropDownMenu::addToInputController();

	/**
	* Remove this object as InputListener from InputController
	*/
	void DropDownMenu::removeFromInputController();


	/**
	This function will create a list of menu items, including the KeyMap::functino and the item text.
	*/
	void addFunctionList(std::vector<std::pair<KeyMap::function, std::string>> funclist);
	
	/**
	* function to retrieve all keys the dropdown menu listens to.                                                                     
	*/
	std::vector<KeyMap::function> InputListener::getKeyMap() override;

	/**
	* Remove the current font used in the DropDownMenu.                                                                     
	*/
	~DropDownMenu();

private:

	const static int maxSize = 20; //max item size.

	sf::Vector2f pos;
	void show();
	int	HoverOverButton();

	sf::Font * currentFont;

	InputListener * menuListener; //the listener the actions will be send back to


	bool isDown;
	int dropSize;
	sf::Font font;
	float hight;
	float with;


	struct dropDownFunction {
		KeyMap::function func;
		sf::Text text;
		sf::RectangleShape shape;
	};

	std::vector <dropDownFunction> dropDownMenuFunctions;

	/*
	This will create a drawable dropdown item menu.
	@param A pair of the keymap::function and a function text string
	*/
	void tempaddFuntion(std::pair<KeyMap::function, std::string> tempFunctionList);



};