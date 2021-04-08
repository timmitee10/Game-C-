#pragma once
#include "GameObject.h"
class Player final : public GameObject
{
public:
	Player(const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1,1))
		: GameObject(texture, pos, rotation, color, scale)
	{
	}
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow* const renderer) const override;
private:
	float velocity = 0.f;
};