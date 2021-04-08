#include "Player.h"

void Player::Update(float deltaTime)
{

	this->velocity = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->velocity = movementSpeed;
		this->rotation = 360;
		GameObject::Update(deltaTime);
		this->position += (direction * velocity * deltaTime * 0.0000001f);
		this->sprite.setPosition(position);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->velocity = movementSpeed;
		this->rotation = 90;

		GameObject::Update(deltaTime);
		this->position += (direction * velocity * deltaTime * 0.0000001f);
		this->sprite.setPosition(position);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->velocity = movementSpeed;
		this->rotation = 180 + 90;

		GameObject::Update(deltaTime);
		this->position += (direction * velocity * deltaTime * 0.0000001f);
		this->sprite.setPosition(position);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->velocity = movementSpeed;
		this->rotation = 180;

		GameObject::Update(deltaTime);
		this->position += (direction * velocity * deltaTime * 0.0000001f);
		this->sprite.setPosition(position);
	}

	this->direction = FindDirection(this->position, sf::Vector2f(sf::Mouse::getPosition()));

}

void Player::Draw(sf::RenderWindow* const renderer) const
{
	renderer->draw(this->sprite);
}