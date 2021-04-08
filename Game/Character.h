#pragma once
#include "GameObject.h"
#include "Inventory.h"
class Character : public GameObject
{
public:
	Character(float health, float velocity, Inventory& inventory,const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : GameObject(texture, pos, rotation, color, scale), inventory(this)
	{
		this->health = health;
		this->velocity = velocity;
		this->inventory = inventory;
	}
	Character(float health, float velocity, Inventory& inventory,GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : GameObject(gameObjects, texture, pos, rotation, color, scale) , inventory(this)
	{
		this->health = health;
		this->velocity = velocity;
		this->inventory = inventory;
	}
	virtual ~Character() {};
	void SetWeapon(unsigned int index) { inventory.UseWeapon(index); }
	Inventory& GetInventory() { return inventory; }


	bool operator==(const GameObject& lhs) const override
	{
		return *this == dynamic_cast<const Character&>(lhs);
	}

	bool operator!=(const GameObject& lhs) const override
	{
		return !this->operator==(lhs);
	}
	/*
	using GameObject::Draw;
	using GameObject::Update;
	using GameObject::Intersects;
	using GameObject::IntersectsTop;
	using GameObject::IntersectsBottom;
	using GameObject::IntersectsLeft;
	using GameObject::IntersectsRight;

	using GameObject::SetRotation;
	using GameObject::SetColor;
	using GameObject::SetPosition;
	
	using GameObject::GetRotation;
	using GameObject::GetColor;
	using GameObject::GetOrigin;
	using GameObject::GetPos;
	using GameObject::GetScale;
	using GameObject::GetTexture;
	using GameObject::GetRect;
	*/
	float GetHealth() const { return health; }
	void SetHealth(const float health) { this->health = health; }
protected:
	float health;
	float velocity;
	Inventory inventory;
	
};
