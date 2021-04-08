#include "Player.h"

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	this->velocity = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->velocity = 1;
		this->rotation = 360;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->velocity = 1;
		this->rotation = 90;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->velocity = 1;
		this->rotation = 180 + 90;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->velocity = 1;
		this->rotation = 180;
	}
	this->position += (direction * velocity * deltaTime * 0.0000001f);
	this->sprite.setPosition(position);
}

void Player::Draw(sf::RenderWindow* const renderer) const
{
	renderer->draw(this->sprite);
}