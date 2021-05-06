#pragma once
#include "GameObject.h"
class Inventory;
class Character : public GameObject
{
public:
	Character(float health, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture,
	          const sf::Vector2f& pos, float rotation,
	          const sf::Vector2f& scale = sf::Vector2f(1, 1));

	//~Character() override;
	void SetWeapon(unsigned int index);
	Inventory* GetInventory();


	bool operator==(const GameObject& lhs) const override;

	bool operator!=(const GameObject& lhs) const override;
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
	float GetHealth() const;
	void SetHealth(const float health);
protected:
	float health = 100.f;
	float velocity = 2.f;
	Inventory* myInventory;
};