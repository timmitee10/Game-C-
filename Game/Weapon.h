#pragma once
#include "Bullet.h"
#include "Character.h"
#include "Character.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "Timer.h"

class Character;


struct WeaponDetails
{
	float reloadTime;
	float fireDelay;
	unsigned int totalMagazineCapacity;
	unsigned int magazineBulletCount = 0;

	sf::Texture* equippedTexture;
	sf::Texture* objectTexture;
};


/* Weapon in inventory */
class Weapon
{
public:
	Weapon(const sf::Texture* texture, GameObjectManager* objectManager, Character* owner, WeaponDetails* details,
	       Bullet* bullet);
	~Weapon() = default;

	void Reload();

	void FillWeapon() const;

	virtual void Shoot();

	void Update();

	void Draw(sf::RenderWindow* render) const
	{
		render->draw(sprite);
	}
protected:
	sf::Sprite sprite;
	WeaponDetails* details;
	GameObjectManager* objectManager;
	Character* owner;
	Timer reloadTimer;
	Timer shootTimer;
	bool isReloading = false;
	Bullet* bullet;
	sf::Vector2f offset;
};


/* Weapon on ground able to be picked up */
class WeaponObject : GameObject
{
public:
	WeaponObject(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		WeaponDetails* details, const sf::Vector2f& scale = sf::Vector2f(1, 1)) : GameObject(gameObjects, texture, pos, rotation, scale), details(details)
	{
		
	}
	
	
protected:
	WeaponDetails* details;
};