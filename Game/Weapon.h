#pragma once
#include "GameObject.h"
class Weapon
{
public:
	Weapon();
	~Weapon();
	void Reload();
	virtual void Shot();
private:
	unsigned int magazineCappacity;
	unsigned int reloadTime;
	GameObject* target;
};