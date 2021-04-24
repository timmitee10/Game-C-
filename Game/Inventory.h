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

constexpr const char* ToString(WeaponType x)
{
	switch (x) {
	case WeaponType::Pistol: return "Pistol";
	case WeaponType::Rifle: return "Rifle";
	default: return "Unknown";
	}
}

//class WeaponContainer : std::vector<std::optional<Weapon>>
//{
//public:
//	
//};
class Inventory final
{
public:
	using WeaponContainer = std::vector<std::optional<Weapon>>;
	unsigned int bandageCount;
	unsigned int bulletCount;

	WeaponContainer::iterator currentWeapon;
	Character* owner;

	Inventory(Character* owner, GameObjectManager* gameObjectManager) : owner(owner), gameObjectManager(gameObjectManager) { weapons.resize(INVENTORY_WEAPON_MAX_COUNT); currentWeapon = weapons.begin(); }
	void UseWeapon(unsigned int index)
	{
		assert(index > INVENTORY_WEAPON_MAX_COUNT && "Index outsize of vector!");
		currentWeapon = weapons.begin() + index;
	}

	void UseNextWeapon()
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

	void PickupWeapon(Weapon&& weapon)
	{
		weapons.emplace(currentWeapon, std::make_optional(weapon));
	}

	void DropWeapon(WeaponContainer::iterator it)
	{
		//////TODO will crash???
		if (it->has_value())
		{
			Weapon* tempWeapon = &it->value();
			WeaponObject* weaponObject = new WeaponObject(tempWeapon.)
			gameObjectManager->Append<WeaponObject>(weaponObject);
			weapons.erase(it);
		}
	}
private:
	WeaponContainer weapons;
	GameObjectManager* gameObjectManager;
};