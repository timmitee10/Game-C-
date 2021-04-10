#pragma once
#include "Player.h"
#include "Bar.h"
typedef GameObject UIElement;

class UserInterface
{
public:
	UserInterface(Character* target, const sf::Texture* uiRifle, const sf::Texture* uiPistol, const sf::Texture* uiBandage) :
	target(target) , inventory(&target->GetInventory())
	{
		target->GetHealth();
		
	}
	~UserInterface()
	{
		
	}
	
	void Draw(sf::RenderWindow* render)
	{
		healthBar.Draw(render);
		
	}
public:
	Bar healthBar;
	Character* target;
	Inventory* inventory; 
	sf::Sprite* rifle;
	sf::Sprite* pistol;
	sf::Sprite* bandage;
};
