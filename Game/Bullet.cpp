#include "Bullet.h"
#include "Character.h"
void Bullet::Update(float deltaTime)
{
	std::vector<std::unique_ptr<GameObject>>* temp = this->objectManager->GetVector();
	for (auto& a : *temp)
	{
		if (this->Intersects(this->GetRect()) && owner->operator!=(*a))
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
}
