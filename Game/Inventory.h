#pragma once
#include <cassert>
#include <vector>

#include "Weapon.h"
#include "Bullet.h"
#include <optional>
#define INVENTORY_WEAPON_MAX_COUNT 2
class Character;

enum class Weapons
{
	Pistol,
	Rifle,
};
struct Inventory
{
	using AWeapon = Weapon<Bullet>;
	using WeaponContainer = std::vector<std::optional<AWeapon>>;
	unsigned int bandageCount;
	unsigned int bulletCount;
	
	WeaponContainer::iterator currentWeapon;
	Character* owner;

	Inventory(Character* owner) : owner(owner) { weapons.resize(INVENTORY_WEAPON_MAX_COUNT); }
	void UseWeapon(unsigned int index)
	{
		assert(index > INVENTORY_WEAPON_MAX_COUNT && "Index outsize of vector!");
		currentWeapon = weapons.begin() + index;
	}
	void PickupWeapon(AWeapon&& weapon)
	{
		weapons.emplace(currentWeapon, weapon);
	}
private:
	WeaponContainer weapons;
};