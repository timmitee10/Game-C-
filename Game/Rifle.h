#pragma once
#include "Bullet.h"
#include "Weapon.h"

class Rifle final : public Weapon
{
public:
	Rifle(sf::Texture* texture, GameObjectManager* objectManager, Character* owner, WeaponDetails* details) : Weapon(texture, objectManager, owner, details)
	{
		
	};

	~Rifle() = default;
};

class RifleObject final : public WeaponObject
{
public:
	RifleObject(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		WeaponDetails* details, const sf::Vector2f& scale = sf::Vector2f(1, 1)) : WeaponObject(gameObjects, texture, pos, rotation, details,scale)
	{

	}

	~RifleObject() = default;
private:
	
};