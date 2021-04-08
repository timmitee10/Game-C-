#pragma once
#include "Character.h"
#include "GameObjectManager.h"
#include "Bullet.h"
#include "Inventory.h"

template<typename TBullet>
class Weapon
{
public:
	Weapon(GameObjectManager* objectManager);
	virtual ~Weapon();
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
			TBullet tempBullet;
			tempBullet.SetPos(owner->GetPos());
			tempBullet.SetRotation(owner->GetRotation());
			objectManager->Append(tempBullet());
			magazineBulletCount--;
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
private:
	unsigned int totalMagazineCapacity;
	unsigned int magazineBulletCount;
	float reloadTime;
	float fireDelay;
	GameObjectManager* objectManager;
	Character* owner;
	Timer reloadTimer;
	Timer shootTimer;
	bool isReloading;
};
