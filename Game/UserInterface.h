#pragma once
#include "Player.h"
typedef GameObject UIElement;
class UserInterface
{
	
public:
	UserInterface(Inventory* inventory)
	{
		inventory->owner->GetHealth();
	}
	~UserInterface()
	{
		
	}
	void Update()
	{

	}
	void Draw(sf::RenderWindow* render)
	{
		
	}
public:
	Inventory* inventory;
	UIElement* rifle;
	UIElement* pistol;
	UIElement* bandage;
};
