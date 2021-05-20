#pragma once
#include "Character.h"

class Bot final : Character
{
	Bot(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1));
	virtual ~Bot() = default;
	
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* const renderer) const override;
private:

	
};
