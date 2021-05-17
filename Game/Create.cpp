#include "Create.h"

Create::Create(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Vector2f& scale) : GameObject(gameObjects, texture, pos, rotation, scale)
{
	for (int i = 0; i < 5; i++)
	{
		if (std::rand() % 2 == 0)
		{
			//weapons.push_back());
		}
	}
}

Create::~Create()
{
}

void Create::Destroy()
{
	for (auto& weapon : this->weapons)
	{
		this->objectManager->Append(std::dynamic_pointer_cast<GameObject>(std::make_shared<WeaponObject>(weapon)));
	}
	this->isDestroyed = true;
}

void Create::Draw(sf::RenderWindow* const renderer) const
{
	if (!isDestroyed)
		renderer->draw(sprite);
}
