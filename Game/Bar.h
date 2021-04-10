#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "GameObject.h"

class Bar final
{
public:
	Bar(const sf::Texture* texture, sf::Vector2f position, float rotation, float& amount, float maxAmount, sf::Vector2f offset, unsigned int width, unsigned int height)
		: width(width), height(height), offset(offset), amount(&amount)
	{
		sprite.setTexture(*texture);
		sprite.setPosition(position);
		sprite.setRotation(rotation);
	};
	~Bar() = default;
	void Update()
	{
		//TODO fix
	}
	void Draw(sf::RenderWindow* const renderer)
	{
		renderer->draw(sprite);
	}
private:
	GameObject* target;
	sf::Sprite sprite;
	unsigned int width;
	unsigned int height;
	float* amount;
	float maxAmount;
	sf::Vector2f offset;
};