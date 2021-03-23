#include "Player.h"

void Player::Update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->position += velocity * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->position += velocity * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->position += velocity * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->position += velocity * deltaTime;
}

void Player::Draw(sf::RenderWindow* const renderer) const
{
	renderer->draw(this->sprite);
}