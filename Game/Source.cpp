#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjectManager.h"
#include "Player.h"
#include "TextureManager.h"
static Player* player;
int main()
{
	auto videoMode = sf::VideoMode::getFullscreenModes();
	sf::RenderWindow window(videoMode[0], "SFML works!");
	auto windowSize = window.getSize();
	auto windowOrigin = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
	sf::CircleShape shape(1000.f);
	GameObjectManager manager(&window);
	shape.setFillColor(sf::Color::Green);
	if (!TextureManager::Load("bird.jpg")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("pistol2.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("player.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("rifle2.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("stone.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("tree.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("box.jpg")) throw std::exception("Faild to load file");
	//manager.Append<Player>(new Player(TextureManager::Get("bird.jpg"), sf::Vector2f(0, 0), 0, sf::Color::White, sf::Vector2f(0.2, 0.2)));
	player = new Player(100, 100, TextureManager::Get("player.png"), windowOrigin, 0);
	manager.Append<Player>(player);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::MouseWheelMoved)
			{
				player->GetInventory().NextWeapon();
				//event.mouseWheel.delta
			}
		}
		manager.UpdateAll();
		window.clear();
		window.draw(shape);
		manager.DrawAll();
		window.display();
	}
	return 0;
}