#pragma once
#include "Character.h"
#include "Inventory.h"
class Player : public Character
{
public:
	Player(float health, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture,
	       const sf::Vector2f& pos, float rotation, 
			sf::RenderWindow* window, Weapon* weapon,
	       const sf::Vector2f& scale = sf::Vector2f(1, 1));
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* const renderer) const override;

	void SetUniqueId(unsigned int value);
private:
	float movementSpeed = 250.f;
	sf::RenderWindow* window;
	Weapon* weapon;
};