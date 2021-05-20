#include "Bot.h"

Bot::Bot(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Vector2f& scale) : 
	Character(health, velocity, gameObjects, texture, pos, rotation, scale)
{

}

void Bot::Update(float deltaTime)
{
	this->objectManager;
	this->position;
	this->rotation;
	this->velocity;
	this->health;
}

void Bot::Draw(sf::RenderWindow* const renderer) const
{

}