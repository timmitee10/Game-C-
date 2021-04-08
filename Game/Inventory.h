#pragma once
#include <cassert>
#include <vector>

#include "Weapon.h"
#include "Bullet.h"
#include <optional>
#define INVENTORY_WEAPON_MAX_COUNT 2
class Character;

enum class WeaponType
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

	Inventory(Character* owner) : owner(owner) { weapons.resize(INVENTORY_WEAPON_MAX_COUNT); currentWeapon = weapons.begin(); }
	void UseWeapon(unsigned int index)
	{
		assert(index > INVENTORY_WEAPON_MAX_COUNT && "Index outsize of vector!");
		currentWeapon = weapons.begin() + index;
	}

	void NextWeapon()
	{
		if (*currentWeapon != std::nullopt)
		{
			const auto next = std::next(currentWeapon, 1);
			if (next->has_value())
			{
				currentWeapon = next;
			}
			else
			{
				return;
			}
		}
	}
	
	void PickupWeapon(AWeapon&& weapon)
	{
		weapons.emplace(currentWeapon, std::make_optional(weapon));
	}


private:
	WeaponContainer weapons;
};