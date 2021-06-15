#pragma once
#include <cassert>
#include <vector>

#include <optional>

#include "Character.h"
#define INVENTORY_WEAPON_MAX_COUNT 2

class Weapon;
class WeaponObject;

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

	Inventory(Character* owner, GameObjectManager* gameObjectManager);

	void UseWeapon(unsigned int index);

	void UseNextWeapon();

	void PickupWeapon(WeaponObject& weaponObject);

	void DropWeapon(WeaponContainer::iterator it);

	//void UseBandage();
private:
	WeaponContainer weapons;
	GameObjectManager* gameObjectManager;
};