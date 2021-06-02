#pragma once
#include "GameObjectManager.h"
class Character;

class Bullet : public GameObject
{
public:
	Bullet(const Character* owner, float damage, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1));
	~Bullet() override = default;
	void Update(float deltaTime) override;
private:
	const Character* owner;
	float damage;
	float velocity;
	sf::Vector2f offset;

	sf::Vector2f orignalPosition;
	float maxDistance = 10000;
};