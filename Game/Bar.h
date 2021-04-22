#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "GameObject.h"

class Bar final
{
public:
	Bar(const sf::Texture* texture, sf::Vector2f position, float rotation, float& amount, float maxAmount, sf::Vector2f offset, unsigned int width, unsigned int height)
		: target(nullptr), width(width), height(height), amount(&amount), maxAmount(maxAmount), offset(offset)
	{
		sprite.setTexture(*texture);
		sprite.setPosition(position);
		sprite.setRotation(rotation);
	};
	~Bar() = default;
	void Update() const
	{
		//TODO fix
	}
	void Draw(sf::RenderWindow* const renderer) const
	{
		renderer->draw(sprite);
	}

	void SetValue(float* ptrAmount)
	{
		amount = ptrAmount;
	}
	float* GetAmount() const
	{
		return amount;
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

