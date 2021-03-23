#include <SFML/Graphics.hpp>
#include "GameObjectManager.h"
#include "Player.h"
#include "TextureManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	GameObjectManager manager(&window);
	shape.setFillColor(sf::Color::Green);
	if(!TextureManager::Load("black-box.jpg")) throw std::exception("Faild to load file");
	manager.Append(new Player(TextureManager::Get("black-box.jpg"), sf::Vector2f(10, 10), 0, sf::Color::Blue, sf::Vector2f(1, 1)));

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
