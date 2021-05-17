#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjectManager.h"
#include "Player.h"
#include "Stone.h"
#include "TextureManager.h"
#include "Tree.h"
inline float RandomRotation()
{
	return (std::rand() % 360);
}

static std::vector<WeaponDetails> avalibleWeapons;
static Bullet* bullet;
static std::shared_ptr<Player> player;
int main()
{
	auto videoMode = sf::VideoMode::getFullscreenModes();
	//sf::RenderWindow window(videoMode[0], "SFML works!");
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	auto windowSize = window.getSize();
	auto windowOrigin = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
	//sf::CircleShape shape(1000.f);
	//shape.setFillColor(sf::Color::Green);
	GameObjectManager manager(&window);
	if (!TextureManager::Load("bird.jpg")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("pistol2.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("player.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("rifle2.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("stone.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("tree.png")) throw std::exception("Faild to load file");
	if (!TextureManager::Load("box.jpg")) throw std::exception("Faild to load file");
	//manager.Append<Player>(new Player(TextureManager::Get("bird.jpg"), sf::Vector2f(0, 0), 0, sf::Color::White, sf::Vector2f(0.2, 0.2)));
	player = std::make_shared<Player>(100, 100, &manager, TextureManager::Get("player.png"), sf::Vector2f(50,50), 0, &window);
	manager.Append(std::dynamic_pointer_cast<GameObject>(player));
	//manager.Append(reinterpret_cast<GameObject*>(new Tree(TextureManager::Get("player.png"), sf::Vector2f(0, 0), 0.f)));
	//manager.Append(reinterpret_cast<GameObject*>(new Stone(TextureManager::Get("stone.png"), sf::Vector2f(0, 0), 0.f)));
	//
	//manager.Append(reinterpret_cast<GameObject*>(new Stone(TextureManager::Get("stone.png"), sf::Vector2f(1900, 1121), 0.f)));
	//manager.Append(reinterpret_cast<GameObject*>(new Stone(TextureManager::Get("stone.png"), sf::Vector2f(1453, 1423), 0.f)));

	//manager.Append(Weapon(, , , ,));
	avalibleWeapons.resize(2);



	sf::View view;
	window.setView(view);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseWheelMoved)
			{
				player->GetInventory()->UseNextWeapon();
				//event.mouseWheel.delta
			}
		}
		manager.UpdateAll();
		window.clear();
		view.setCenter(player->GetPos());
		//window.draw(shape);
		manager.DrawAll();
		window.display();
	}
	return 0;
}