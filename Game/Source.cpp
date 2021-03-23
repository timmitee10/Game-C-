#include <SFML/Graphics.hpp>
#include "GameObjectManager.h"
#include "Player.h"
#include "TextureManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    GameObjectManager manager(window);
    shape.setFillColor(sf::Color::Green);
    TextureManager::Load("idk");
    manager.Append(new Player(TextureManager::Get("idk"), sf::Vector2f(10, 10), 0, sf::Color::Blue, sf::Vector2f(1, 1)));
	
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
        manager.DrawAll();
        window.draw(shape);
        window.display();
    }

	

    return 0;
}
