#pragma once
#include "GameObject.h"
class Bullet : GameObject
{
public:
	Bullet();
	~Bullet();
	void Update(float deltaTime) override
	{
		
	}
private:
	float damage;
	float speed;
};
