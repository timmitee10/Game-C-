#include "Bot.h"
#include "GameObjectManager.h"
Bot::Bot(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Vector2f& scale) :
	Character(health, velocity, gameObjects, texture, pos, rotation, scale)
{

}

void Bot::Update(float deltaTime)
{
	std::vector<std::shared_ptr<GameObject>>& temp = *this->objectManager->GetVector();
	for (auto& a : temp)
	{


		this->isRemoved = true;
	}




}

void Bot::Draw(sf::RenderWindow* const renderer) const
{

}