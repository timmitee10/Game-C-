#pragma once
#include "Bullet.h"
#include "GameObjectManager.h"
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

	Bullet* bullet;
}; 

/* Weapon in myInventory */
class Weapon 
{
	using Owner = Character;
public:
	Weapon(GameObjectManager* objectManager, WeaponDetails* details);
	Weapon(GameObjectManager* objectManager, Owner* owner,WeaponDetails* details);

	virtual ~Weapon() { };

	void Reload();

	void FillWeapon() const;

	void Shoot();

	void Update();

	void Draw(sf::RenderWindow* render) const;

	WeaponDetails* GetWeaponDetails() const;

	void SetOwner(Owner* owner);
protected:
	sf::Sprite sprite;
	WeaponDetails* details;
	GameObjectManager* objectManager;
	Owner* owner = nullptr;
	Timer reloadTimer;
	Timer shootTimer;
	bool isReloading = false;
	//Bullet* bullet = nullptr;
	sf::Vector2f offset;
};

/* Weapon on ground able to be picked up */
class WeaponObject : public GameObject
{
public:
	WeaponObject(GameObjectManager* gameObjectManager, WeaponDetails* details, Character* target);

	WeaponDetails* GetDetails() const;
protected:
	WeaponDetails* details;
};