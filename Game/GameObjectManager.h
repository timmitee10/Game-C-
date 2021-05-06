#pragma once
#include <vector>
#include "GameObject.h"
#include "Timer.h"

class GameObjectManager
{
public:
	using GameObjectContainer = std::vector<std::unique_ptr<GameObject>>;
	GameObjectManager(sf::RenderWindow* renderer);;
	~GameObjectManager() = default;
public:
	void DrawAll() const;

	void UpdateAll();

	void Remove(const GameObject* ptr)
	{
		//gameObjects.erase(objectMemoryMap.at(ptr));
	}

	void Append(const GameObject* obj);

	std::vector<std::unique_ptr<GameObject>>* GetVector();
private:
	sf::RenderWindow* renderer;
	GameObjectContainer gameObjects;
	Timer timer;
	/* Memory address of object to object is vector */
	//std::map<const GameObject*, GameObjectContainer::iterator> objectMemoryMap;
};