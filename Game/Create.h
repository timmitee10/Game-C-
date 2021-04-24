#pragma once
#include "GameObject.h"
#include "Weapon.h"

#include "Bullet.h"

class Create final: GameObject
{
public:
	Create(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1)) : GameObject(gameObjects, texture, pos, rotation, scale)
	{
		for (int i = 0; i < 5; i++)
		{
			if (std::rand() % 2 == 0)
			{
				//weapons.push_back());
			}
		}
	}
	virtual ~Create();
	void Destroy();
	void Draw(sf::RenderWindow* const renderer) const override
	{
		if (!isDestroyed)
			renderer->draw(sprite);
		else
		{
			for(auto& temp : weapons)
			{
				
			}
		}
	}
private:
	std::vector<Weapon> weapons;
	bool isDestroyed = false;
};
