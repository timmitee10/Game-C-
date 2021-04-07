#pragma once
#include <vector>
#include "Bandage.h"
enum class Weapons
{
	Pistol,
	Rifle,
};
struct Inventory
{
	unsigned int bandageCount;
	unsigned int bulletCount;
	Weapons usedWeapon;
};