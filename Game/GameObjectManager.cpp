#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(sf::RenderWindow* renderer)
{
	this->renderer = renderer;
}

void GameObjectManager::DrawAll() const
{
	for (const auto& object : gameObjects)
	{

		if (object.get() != nullptr)
		{
			object->Draw(renderer);
		}
	}
}

void GameObjectManager::UpdateAll()
{
	/* Update */
	//Iterator get invalid after inserting object into vector therefore forlook was used instead.
	for (int  i = gameObjects.size() - 1; i >= 0; i--)
	{
		auto& object = gameObjects.at(i);

		if (object.get() != nullptr)
		{
			object->Update(timer.ElapsedNanoseconds());

			//if (object->IsRemoved())
			//{
			//	gameObjects.erase(gameObjects.begin() + i);
			//	//gameObjects.erase(std::next(object).base());
			//}
		}

	}
	/* Remove */
	for (int i = gameObjects.size() - 1; i >= 0; i--)
	{
		auto& object = gameObjects.at(i);
		if (object.get() != nullptr)
		{
			if (object->IsRemoved())
			{
				gameObjects.erase(gameObjects.begin() + i);
			}
		}
	}
}
void GameObjectManager::Remove(GameObject* ptr)
{
	//gameObjects.erase(objectMemoryMap.at(ptr));
}
void GameObjectManager::Append(std::shared_ptr<GameObject> obj)
{
	gameObjects.push_back(obj);
}

std::shared_ptr<GameObject> GameObjectManager::GetObjectById(uint64_t objectId)
{
	for (auto& a : gameObjects)
	{
		if (a->GetId() == objectId)
			return a;
	}
	throw std::exception("Could not find object");
}

GameObjectManager::GameObjectContainer* GameObjectManager::GetVector()
{
	return &gameObjects;
}