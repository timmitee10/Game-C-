#pragma once
#include "GameObjectManager.h"
class Character;

class Bullet : public GameObject
{
public:
	Bullet(const Character* owner, float damage, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1, 1))
		: GameObject(gameObjects, texture, pos, rotation, color, scale) {
		this->owner = owner; 
		this->damage = damage;
		this->velocity = velocity;
	};
	virtual ~Bullet() = default;
	void Update(float deltaTime) override
	{
		//std::vector<std::unique_ptr<GameObject>>* temp = gameObjectManager->GetVector();
		//for (auto& a : *temp)
		//{
		//	if (this->Intersects(this->GetRect()) && owner->!=(*a))
		//	{
		//		auto* p = dynamic_cast<Character*>(a.get());
		//		if(p)
		//		{
		//			//auto* b = reinterpret_cast<Character*>(a.get());
		//			float tempNewHealth = p->GetHealth() - damage;
		//			p->SetHealth(tempNewHealth);
		//		}
		//		else
		//		{
		//			this->IsRemoved = true;
		//		}
		//	}
		//}
	}
private:
	GameObjectManager* gameObjectManager;
	const Character* owner;
	float damage;
	float velocity;
};
