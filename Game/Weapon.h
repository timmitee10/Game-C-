#pragma once
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "Timer.h"

class Character;

template<typename TBullet>
class Weapon
{
public:
	Weapon(sf::Texture* texture, GameObjectManager* objectManager, Character* owner, float reloadTime, float fireDelay, float totalMagazineCapacity) :
		texture(texture), objectManager(objectManager), owner(owner), reloadTime(reloadTime), fireDelay(fireDelay), totalMagazineCapacity(totalMagazineCapacity) {};
	virtual ~Weapon() = default;

	void Reload()
	{
		if (!isReloading)
		{
			reloadTimer.Reset();
			isReloading = true;
		}
	}

	void FillWeapon()
	{
		if (owner->GetInventory().bulletCount >= totalMagazineCapacity)
		{
			magazineBulletCount = totalMagazineCapacity;
			owner->GetInventory().bulletCount -= totalMagazineCapacity;
		}
		else if (owner->GetInventory().bulletCount > 0)
		{
			magazineBulletCount = owner->GetInventory().bulletCount;
			owner->GetInventory().bulletCount = 0;
		}
	}
	virtual void Shoot()
	{
		if (!isReloading)
		{
			if (shootTimer.ElapsedMilliseconds() > fireDelay)
			{
				TBullet tempBullet(owner, 10.f, 10.f, objectManager, TextureManager::Get("Bullet"), owner->GetPos(), owner->GetRotation());
				tempBullet.SetPosition(owner->GetPos());
				tempBullet.SetRotation(owner->GetRotation());
				objectManager->Append<GameObject>(static_cast<const GameObject*>(&tempBullet));
				magazineBulletCount--;
				shootTimer.Reset();
			}
		}
	}
	void Update()
	{
		if (isReloading)
		{
			if (reloadTimer.ElapsedMilliseconds() < reloadTime)
			{
				return;
			}
			else //Is done reloading
			{
				isReloading = false;
				FillWeapon();
			}
		}
	}
	void Draw()
	{
		
	}
protected:
	sf::Texture* equipedTexture;
	sf::Texture* objectTexture;
	sf::Texture* texture;
	unsigned int totalMagazineCapacity;
	unsigned int magazineBulletCount = 0;
	float reloadTime;
	float fireDelay;
	GameObjectManager* objectManager;
	Character* owner;
	Timer reloadTimer;
	Timer shootTimer;
	bool isReloading = false;
};
