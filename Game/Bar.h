#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "GameObject.h"

class Bar
{
public:
	Bar(GameObject* target,sf::Vector2f offset,unsigned int width,unsigned int height) : width(width), height(height), offset(offset) {};
	~Bar() = default;
	void Draw(sf::RenderWindow* const renderer)
	{

	}
private:
	GameObject* target;
	sf::Texture* texture;
	unsigned int width;
	unsigned int height;
	sf::Vector2f offset;
};

class HealthBar
{
	HealthBar();
	~HealthBar();
	
};