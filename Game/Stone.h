#pragma once
#include "GameObject.h"

class Stone final: GameObject
{
public:
	Stone(const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : GameObject(texture, pos, rotation, scale)
	{
		
	}

	
};
