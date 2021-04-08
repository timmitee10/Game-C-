#pragma once
#include "GameObjectManager.h"
class Character;

class Bullet : public GameObject
{
public:
	Bullet(const Character* owner, float damage, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1))
		: GameObject(gameObjects, texture, pos, rotation, scale) {
		this->owner = owner; 
		this->damage = damage;
		this->velocity = velocity;
	};
	virtual ~Bullet() = default;
	void Update(float deltaTime) override;
private:
	GameObjectManager* gameObjectManager;
	const Character* owner;
	float damage;
	float velocity;
};
