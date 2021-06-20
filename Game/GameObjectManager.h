#pragma once
#include <vector>
#include "GameObject.h"
#include "Timer.h"

class GameObjectManager
{
public:
	using GameObjectContainer = std::vector<std::shared_ptr<GameObject>>;
	GameObjectManager(sf::RenderWindow* renderer);;
	~GameObjectManager() = default;
public:
	void DrawAll() const;

	void UpdateAll();

	void Remove(GameObject* ptr);
	void Append(std::shared_ptr<GameObject> obj);

	std::shared_ptr<GameObject> GetObjectById(uint64_t objectId);
	GameObjectContainer* GetVector();
private:
	sf::RenderWindow* renderer;
	GameObjectContainer gameObjects;
	Timer timer;
	/* Index used for fast lookup of game object, the index is equal to the start memory location of the gameobject and is linked to the index in the vector. Allowing for super fast lookup */
	std::map<const GameObject*, GameObjectContainer::iterator> objectMemoryIndex; 

	/* Memory address of object to object is vector */
	//std::map<const GameObject*, GameObjectContainer::iterator> objectMemoryMap;
};