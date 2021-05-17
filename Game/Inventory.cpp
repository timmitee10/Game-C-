#include "Inventory.h"
#include "Weapon.h"

Inventory::Inventory(Character* owner, GameObjectManager* gameObjectManager): bandageCount(0), bulletCount(0),
                                                                              owner(owner),
                                                                              gameObjectManager(gameObjectManager)
{
	weapons.resize(INVENTORY_WEAPON_MAX_COUNT);
	currentWeapon = weapons.begin();
}

void Inventory::UseWeapon(unsigned int index)
{
	assert(index > INVENTORY_WEAPON_MAX_COUNT && "Index outsize of vector!");
	currentWeapon = weapons.begin() + index;
}

void Inventory::UseNextWeapon()
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
			currentWeapon = this->weapons.begin();
		}
	}
}

void Inventory::PickupWeapon(WeaponObject& weaponObject)
{
	Weapon* weapon = new Weapon(gameObjectManager, owner, weaponObject.GetDetails());
	weapons.emplace(currentWeapon, std::make_optional(*weapon));
}

void Inventory::DropWeapon(WeaponContainer::iterator it)
{
	//////TODO will crash???
	if (it->has_value())
	{
		Weapon* tempWeapon = &it->value();
		WeaponObject* weaponObject = new WeaponObject(gameObjectManager, tempWeapon->GetWeaponDetails(), owner);
		gameObjectManager->Append(std::dynamic_pointer_cast<GameObject>(std::shared_ptr<WeaponObject>(weaponObject)));
		weapons.erase(it);
	}
}