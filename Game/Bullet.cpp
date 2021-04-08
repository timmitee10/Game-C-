#include "Bullet.h"
#include "Character.h"
inline void Bullet::Update(float deltaTime)
{
	std::vector<std::unique_ptr<GameObject>>* temp = gameObjectManager->GetVector();
	for (auto& a : *temp)
	{
		if (this->Intersects(this->GetRect()) && owner->operator!=(*a))
		{
			auto* p = dynamic_cast<Character*>(a.get());
			if (p)
			{
				//auto* b = reinterpret_cast<Character*>(a.get());
				float tempNewHealth = p->GetHealth() - damage;
				p->SetHealth(tempNewHealth);
			}
			else
			{
				this->isRemoved = true;
			}
		}
	}
}
