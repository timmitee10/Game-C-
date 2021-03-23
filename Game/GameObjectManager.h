#pragma once
#include <vector>
#include "GameObject.h"
#include "Timer.h"

class GameObjectManager
{
public:
	GameObjectManager(sf::RenderWindow& renderer)
	{
		this->renderer = std::make_shared<sf::RenderWindow>(renderer);
	};
	~GameObjectManager() = default;
public:
	void DrawAll() const
	{
		for (const auto& object : gameObjects)
		{
			object->Draw(renderer.get());
		}
	}
	void UpdateAll()
	{
		for (auto object = gameObjects.rbegin(); object != gameObjects.rend(); ++object)
		{
			object->get()->Update(timer.ElapsedSeconds());
		}
	}
	void Append(const GameObject* obj)
	{
		gameObjects.push_back(std::make_unique<GameObject>(*obj));
	}
private:
	std::shared_ptr<sf::RenderWindow> renderer;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	Timer timer;
};