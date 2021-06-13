#include "Weapon.h"
#include "Character.h"
#include "TextureManager.h"
#include "Inventory.h"
Weapon::Weapon(GameObjectManager* objectManager, WeaponDetails* details) : details(details), objectManager(objectManager)
{
	this->sprite.setTexture(*details->equippedTexture, true);
	//this->sprite.setPosition(owner->GetPos());
	//this->sprite.setRotation(owner->GetRotation());
	this->sprite.setOrigin(0, 0);
}

Weapon::Weapon(GameObjectManager* objectManager, Owner* owner, WeaponDetails* details) : Weapon(objectManager, details)
{
	this->owner = owner;
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
	if (owner->GetInventory()->bulletCount >= details->totalMagazineCapacity)
	{
		details->magazineBulletCount = details->totalMagazineCapacity;
		owner->GetInventory()->bulletCount -= details->totalMagazineCapacity;
	}
	else if (owner->GetInventory()->bulletCount > 0)
	{
		details->magazineBulletCount = owner->GetInventory()->bulletCount;
		owner->GetInventory()->bulletCount = 0;
	}
}

void Weapon::Shoot()
{
	if (!isReloading)
	{
		if (shootTimer.ElapsedMilliseconds() > details->fireDelay)
		{
			float rotation = owner->GetRotation();
			
			Bullet* bu = new Bullet(owner, 10.f, 50.f, objectManager, TextureManager::Get("stone.png"), owner->GetPos(), rotation);
			auto tempBullet = std::make_shared<Bullet>(owner, 10.f, 10.f, objectManager, TextureManager::Get("stone.png"),
				owner->GetPos(), owner->GetRotation());
			tempBullet->SetPosition(owner->GetPos());
			tempBullet->SetRotation(owner->GetRotation());
			objectManager->Append(std::dynamic_pointer_cast<GameObject>(tempBullet));
			details->magazineBulletCount--;
			shootTimer.Reset();
		}
	}
}

void Weapon::Update()
{
	this->sprite.setPosition(owner->GetPos());
	this->sprite.setRotation(owner->GetRotation());
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

void Weapon::Draw(sf::RenderWindow* render) const
{
	render->draw(sprite);
}

WeaponDetails* Weapon::GetWeaponDetails() const
{
	return details;
}

void Weapon::SetOwner(Owner* owner) { this->owner = owner; }

WeaponObject::WeaponObject(GameObjectManager* gameObjectManager, WeaponDetails* details, Character* target) :
	GameObject(gameObjectManager, details->objectTexture, target->GetPos(), target->GetRotation()), details(details)
{
}

WeaponDetails* WeaponObject::GetDetails() const
{
	return details;
}
