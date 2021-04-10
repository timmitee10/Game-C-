#include "Weapon.h"
#include "Character.h"
Weapon::Weapon(const sf::Texture* texture, GameObjectManager* objectManager, Character* owner, WeaponDetails* details,
	Bullet* bullet)
{
	this->sprite.setTexture(*texture, true);
	this->sprite.setPosition(owner->GetPos());
	this->sprite.setRotation(owner->GetRotation());
	this->sprite.setOrigin(0, 0);
	
}

void Weapon::Reload()
{
	if (!isReloading)
	{
		reloadTimer.Reset();
		isReloading = true;
	}
}

void Weapon::FillWeapon() const
{
	if (owner->GetInventory().bulletCount >= details->totalMagazineCapacity)
	{
		details->magazineBulletCount = details->totalMagazineCapacity;
		owner->GetInventory().bulletCount -= details->totalMagazineCapacity;
	}
	else if (owner->GetInventory().bulletCount > 0)
	{
		details->magazineBulletCount = owner->GetInventory().bulletCount;
		owner->GetInventory().bulletCount = 0;
	}
}

void Weapon::Shoot()
{
	if (!isReloading)
	{
		if (shootTimer.ElapsedMilliseconds() > details->fireDelay)
		{
			Bullet* tempBullet = new Bullet(owner, 10.f, 10.f, objectManager, TextureManager::Get("Bullet"),
			                                owner->GetPos(), owner->GetRotation());
			tempBullet->SetPosition(owner->GetPos());
			tempBullet->SetRotation(owner->GetRotation());
			objectManager->Append<GameObject>(dynamic_cast<const GameObject*>(tempBullet));
			details->magazineBulletCount--;
			shootTimer.Reset();
		}
	}
}

void Weapon::Update()
{
	if (isReloading)
	{
		if (reloadTimer.ElapsedMilliseconds() < details->reloadTime)
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
