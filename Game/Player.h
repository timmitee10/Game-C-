#pragma once
#include "Character.h"
#include "Inventory.h"
class Player final : public Character
{
public:
	Player(float health, float velocity, Inventory& inventory, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : Character(health, velocity, inventory, texture, pos, rotation, color, scale)
	{
	}
	Player(float health, float velocity, Inventory& inventory, GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : Character(health, velocity, inventory, gameObjects, texture, pos, rotation, color, scale)
	{
	}
	virtual ~Player() {}
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* const renderer) const override;
private:


};