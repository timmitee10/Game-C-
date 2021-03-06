#pragma once
#include "Player.h"
#include "Bar.h"
typedef GameObject UIElement;

class UserInterface
{
public:
	UserInterface(Character* target, const sf::Texture* uiRifle, const sf::Texture* uiPistol, const sf::Texture* uiBandage);

	~UserInterface();
	
	void Draw(sf::RenderWindow* render);
public:
	Bar* healthBar;
	Character* target;
	Inventory* inventory; 
	sf::Sprite* rifle;
	sf::Sprite* pistol;
	sf::Sprite* bandage;
};