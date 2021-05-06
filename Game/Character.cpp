#include "Character.h"
#include "Inventory.h"
Character::Character(float health, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture,
	const sf::Vector2f& pos, float rotation, const sf::Vector2f& scale) :
	GameObject(gameObjects, texture, pos, rotation, scale), health(health), velocity(velocity)
{
	myInventory = new Inventory(this, gameObjects);
}

void Character::SetWeapon(unsigned int index)
{
	myInventory->UseWeapon(index);
}

Inventory* Character::GetInventory()
{
	return myInventory;
}

bool Character::operator==(const GameObject& lhs) const
{
	return *this == dynamic_cast<const Character&>(lhs);
}

bool Character::operator!=(const GameObject& lhs) const
{
	return !this->operator==(lhs);
}

float Character::GetHealth() const
{
	return health;
}

void Character::SetHealth(const float health)
{
	this->health = health;
}
