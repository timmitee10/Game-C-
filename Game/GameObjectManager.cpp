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

void GameObjectManager::Append(const GameObject* obj)
{
	gameObjects.push_back(std::make_unique<GameObject>(*obj));
	/* https://en.cppreference.com/w/cpp/iterator/end */
	//objectMemoryMap.insert({ obj, gameObjects.end() - 1 });
}

std::vector<std::unique_ptr<GameObject>>* GameObjectManager::GetVector()
{
	return &gameObjects;
}
