#pragma once
#include "Weapon.h"
#include "Bullet.h"

class Pistol final : public Weapon
{
	Pistol(sf::Texture* texture, GameObjectManager* objectManager, Character* owner, WeaponDetails* details) : Weapon(texture, objectManager, owner, details)
	{
		
	};
};

class PistolObject final : public WeaponObject
{
public:
	PistolObject(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		WeaponDetails* details, const sf::Vector2f& scale = sf::Vector2f(1, 1)) : WeaponObject(gameObjects, texture, pos, rotation, details, scale)
	{

	}
};