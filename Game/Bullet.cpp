#include "Bullet.h"
#include "Character.h"
Bullet::Bullet(const Character* owner, float damage, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Vector2f& scale)
	: GameObject(gameObjects, texture, pos, rotation, scale) {
	this->owner = owner;
	this->damage = damage;
	this->velocity = velocity;
	this->orignalPosition = this->position;
}
void Bullet::Update(float deltaTime)
{
	std::vector<std::shared_ptr<GameObject>>* temp = this->objectManager->GetVector();
	for (auto& a : *temp)
	{
		if (this->Intersects(this->GetHitBox()) && typeid(Character) == typeid(a))
		{
			auto p = std::dynamic_pointer_cast<Character>(a);
			if (p.get() != owner)
			{

			}
			if (p)
			{
				const float tempNewHealth = p->GetHealth() - damage;
				p->SetHealth(tempNewHealth);
			}
			this->isRemoved = true;
		}
	}

	this->AddForce(this->rotation, this->velocity, deltaTime);
	if (Vector2Distance<float>(this->orignalPosition, this->position) > maxDistance)
	{
		this->isRemoved = true;
	}
}
