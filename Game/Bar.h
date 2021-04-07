#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "GameObject.h"

class Bar
{
public:
	Bar(GameObject* target,unsigned int width,unsigned int height);
	~Bar() = default;
	void Draw(sf::RenderWindow* const renderer)
	{

	}
private:
	GameObject* target;
	sf::Texture* texture;
	unsigned int width;
	unsigned int hieght;
};