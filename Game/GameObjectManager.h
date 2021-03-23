#pragma once
#include <vector>
#include "GameObject.h"
#include "Timer.h"

class GameObjectManager
{
public:
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
		for (auto object = gameObjects.rbegin(); object != gameObjects.rend(); ++object)
		{
			object->get()->Update(timer.ElapsedSeconds());
		}
	}

	template<typename T>
	void Append(const T* obj)
	{
		gameObjects.push_back(std::make_unique<T>(*obj));
	}
private:
	sf::RenderWindow* renderer;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	Timer timer;
};