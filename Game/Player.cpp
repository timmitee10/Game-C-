#include "Player.h"

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->rotation = 180;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->rotation = 280;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->rotation = 90;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->rotation = 180;
	this->position += (direction * velocity * deltaTime);
}

void Player::Draw(sf::RenderWindow* const renderer) const
{
	renderer->draw(this->sprite);
}