#pragma once
#include <vector>
#include "GameObject.h"
#include "Timer.h"

class GameObjectManager
{
public:
	using GameObjectContainer = std::vector<std::unique_ptr<GameObject>>;
	GameObjectManager(sf::RenderWindow* renderer)
	{
		this->renderer = renderer;
	};
	~GameObjectManager() = default;
public:
	void DrawAll() const
	{
		for (const auto& object : gameObjects)
		{
			object->Draw(renderer);
		}
	}
	void UpdateAll()
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
	void Remove(const GameObject* ptr)
	{
		//gameObjects.erase(objectMemoryMap.at(ptr));
	}
	
	template<typename T>
	void Append(const T* obj)
	{
		gameObjects.push_back(std::make_unique<T>(*obj));
		/* https://en.cppreference.com/w/cpp/iterator/end */
		//objectMemoryMap.insert({ obj, gameObjects.end() - 1 });

	}

	std::vector<std::unique_ptr<GameObject>>* GetVector()
	{
		return &gameObjects;
	}
private:
	sf::RenderWindow* renderer;
	GameObjectContainer gameObjects;
	Timer timer;
	/* Memory address of object to object is vector */
	//std::map<const GameObject*, GameObjectContainer::iterator> objectMemoryMap;
};