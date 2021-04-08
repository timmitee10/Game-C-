#pragma once
#include "GameObject.h"
#include "Bullet.h"
template<typename TBullet>
class Weapon
{
public:
	Weapon();
	~Weapon();
	void Reload();
	virtual void Shot()
	{
		TBullet(sf::Vector2f(0, 0));
	}
private:
	unsigned int magazineCapacity;
	unsigned int reloadTime;
	GameObject* owner;
};