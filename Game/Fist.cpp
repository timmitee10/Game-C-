#include "Fist.h"
#include "Character.h"
Fist::Fist(const Character* owner, float damage, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Vector2f& scale)
	: GameObject(gameObjects, texture, pos, rotation, scale) {
	this->owner = owner;
	this->damage = damage;
	this->velocity = velocity;

	this->orignalPosition = this->position;
}
void Fist::Update(float deltaTime)
{
	std::vector<std::shared_ptr<GameObject>>* temp = this->objectManager->GetVector();
	for (auto& a : *temp)
	{
		if (this->Intersects(this->GetHitBox()) && owner->operator!=(*a))
		{
			auto* p = dynamic_cast<Character*>(a.get());
			if (p)
			{
				const float tempNewHealth = p->GetHealth() - damage;
				p->SetHealth(tempNewHealth);
			}
			this->isRemoved = true;
		}
	}
	this->AddForce(this->rotation, this->velocity, deltaTime);

	if (Vector2Distance<float>( this->orignalPosition , this->position) > maxDistance)
	{
		this->isRemoved = true;
	}
}
