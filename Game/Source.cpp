#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjectManager.h"
#include "Player.h"
#include "TextureManager.h"

int main()
{
	auto videoMode = sf::VideoMode::getFullscreenModes();
	sf::RenderWindow window(videoMode[0], "SFML works!");
	sf::CircleShape shape(100.f);
	GameObjectManager manager(&window);
	shape.setFillColor(sf::Color::Green);
	if (!TextureManager::Load("bird.jpg")) throw std::exception("Faild to load file");
	manager.Append<Player>(new Player(TextureManager::Get("bird.jpg"), sf::Vector2f(0, 0), 0, sf::Color::White, sf::Vector2f(0.2, 0.2)));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		manager.UpdateAll();
		window.clear();
		window.draw(shape);
		manager.DrawAll();
		window.display();
	}

	return 0;
}
