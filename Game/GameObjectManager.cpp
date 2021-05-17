#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(sf::RenderWindow* renderer)
{
	this->renderer = renderer;
}

void GameObjectManager::DrawAll() const
{
	for (const auto& object : gameObjects)
	{
		object->Draw(renderer);
	}
}

void GameObjectManager::UpdateAll()
{
	/* Update */
	for (auto object = gameObjects.rbegin(); object != gameObjects.rend(); ++object)
	{
		object->get()->Update(timer.ElapsedNanoseconds());
		if (object->get()->IsRemoved())
		{
			gameObjects.erase(std::next(object).base());
		}
	}

	/* Remove */
	for (auto object = gameObjects.rbegin(); object != gameObjects.rend(); ++object)
	{
		if (object->get()->IsRemoved())
		{
			gameObjects.erase(std::next(object).base());
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

GameObjectManager::GameObjectContainer* GameObjectManager::GetVector()
{
	return &gameObjects;
}
