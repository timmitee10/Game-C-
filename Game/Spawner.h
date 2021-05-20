#pragma once
#include "GameObjectManager.h"
class Spawner
{
public:
	Spawner(GameObjectManager* manger, GameObject* object);
	~Spawner() = default;
public:
	void Update(float deltaTime);
	void OnGameObjectDestroyed(std::shared_ptr<GameObject> gameObject) const;
private:
	GameObjectManager* manager;
	GameObject* object;
	struct 
	{
		unsigned int maxEnemyCount;

		unsigned int enemysDestroyed;
		
		unsigned int totalEnemysCount;
	} info;
};