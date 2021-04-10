#pragma once
#include "Character.h"
#include "Inventory.h"
class Player final : public Character
{
public:
	Player(float health, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture,
	       const sf::Vector2f& pos, float rotation,
	       const sf::Vector2f& scale = sf::Vector2f(1, 1));
	virtual ~Player() {}
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* const renderer) const override;
private:
	float movementSpeed = 5.f;
};