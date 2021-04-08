#pragma once
#include "Inventory.h"

class Character
{
public:
	Character();
	~Character();
	
	void SetWeapon(Weapons weapon);
protected:
	float health;
	float velocity;
	Inventory inventory;
};
