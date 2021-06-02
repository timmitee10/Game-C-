#include "Player.h"
#include <iostream>

Player::Player(float health, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture,
	const sf::Vector2f& pos, float rotation, sf::RenderWindow* window, Weapon* weapon, const sf::Vector2f& scale) : Character(
		health, velocity, gameObjects, texture, pos, rotation, scale), window(window), weapon(weapon)
{

}


void Player::Update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->AddForce(360, movementSpeed, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->AddForce(90, movementSpeed, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->AddForce(180 + 90, movementSpeed, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->AddForce(180, movementSpeed, deltaTime);
	}


	sf::Vector2f curPos = this->position;
	auto a = sf::Mouse::getPosition();
	auto w = window->getPosition();
	auto center = sf::Vector2i(w.x + (window->getSize().x / 2), w.y + (window->getSize().y/2));
	auto mouse = sf::Vector2f(center.x - a.x, center.y - a.y);
	float dx = curPos.x - mouse.x;
	float dy = curPos.y - mouse.y;
	std::cout << mouse.x << ":" << mouse.y << std::endl; 
	this->rotation = (atan2(dy, dx)) * 180 / PI + 90;
	sprite.setRotation(rotation);


	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		weapon->Shoot();
		weapon->Update();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		weapon->Reload();
	}
}

void Player::Draw(sf::RenderWindow* const renderer) const
{
	renderer->draw(this->sprite);
}