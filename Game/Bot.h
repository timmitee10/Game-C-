#pragma once
#include "Character.h"

class Bot final : Character
{
	Bot(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : Character(health, velocity, gameObjects, texture, pos, rotation, scale) {}
	~Bot() = default;
	
private:

	
};
