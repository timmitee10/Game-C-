#pragma once
#include "Player.h"
typedef GameObject UIElement;

class UserInterface
{
public:
	UserInterface(Inventory* inventory, const sf::Texture* uiRifle, const sf::Texture* uiPistol, const sf::Texture* uiBandage)
	{
		inventory->owner->GetHealth();
		inventory->
	}
	~UserInterface()
	{
		
	}
	
	void Draw(sf::RenderWindow* render)
	{
		
	}
public:
	Inventory* inventory;
	sf::Sprite* rifle;
	sf::Sprite* pistol;
	sf::Sprite* bandage;
};
