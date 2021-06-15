#include <iostream>
#include <SFML/Graphics.hpp>
//#include "GameObjectManager.h"
#include "TextureManager.h"
#include "Weapon.h"
#include "Player.h"

//#include "Server.h"

inline float RandomRotation()
{
	return (std::rand() % 360 + 1);
}
static sf::Rect<float>* mapBounderes;
inline void SpawnObject(GameObjectManager* manager, Player* player, std::shared_ptr<GameObject> object, unsigned int count)
{
	unsigned int i = 0;
	while (i < count)
	{
		object->SetPosition(sf::Vector2f(std::rand() % (int)mapBounderes->width,
			std::rand() % (int)mapBounderes->height));
		if (Vector2Distance(object->GetPos(), player->GetPos()) > 1000)
		{
			manager->Append(object);
			i++;
		}
		else
			continue;
	}
}
//#define SERVER 1
//#define CLIENT 1
static std::vector<WeaponDetails> avalibleWeapons;
static Bullet* bullet;
static std::shared_ptr<Player> player;
static Weapon* basicWeapon;
std::vector<uint8_t> buffer;

//static NodelNet::Socket* mySocket;
int main()
{
	srand(time(0));
	mapBounderes = new sf::Rect<float>(0, 0, 10000, 10000);

	//using namespace NodelNet;
	//IPEndPoint ipEnd(IPAddressV4::Loopback());
	//mySocket = new Socket(IPVersion::IPv4);
	//mySocket->Connect(ipEnd);

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
	WeaponDetails details;
	details.bullet = bullet;
	details.fireDelay = 300.f;
	details.magazineBulletCount = 30;
	details.totalMagazineCapacity = 30;
	details.equippedTexture = (sf::Texture*)TextureManager::Get("rifle2.png");
	details.objectTexture;
	details.reloadTime = 2.f;

	basicWeapon = new Weapon(&manager, &details);
	player = std::make_shared<Player>(100, 100, &manager, TextureManager::Get("player.png"), sf::Vector2f(0, 0), 0, &window, basicWeapon);
	basicWeapon->SetOwner(std::dynamic_pointer_cast<Character>(player).get());
	manager.Append(std::dynamic_pointer_cast<GameObject>(player));

	//manager.Append(std::dynamic_pointer_cast<GameObject>(std::make_shared<Tree>(TextureManager::Get("player.png"), sf::Vector2f(0, 0), 0.f)));
#ifdef SERVER
	Server server;
	server.Initialize(IPEndPoint(IPAddressV4::Loopback(), 29942));
#endif
#ifdef CLIENT
	Socket client;
	client.Connect(IPEndPoint(IPAddressV4::Loopback(), 29942));
#endif
	//manager.Append(std::make_shared<GameObject>(TextureManager::Get("box.jpg"), sf::Vector2f(200, 0), 0.f));

	buffer.resize(sizeof(Player) * 1000);
	//manager.Append(std::dynamic_pointer_cast<GameObject>(std::make_shared<Stone>(TextureManager::Get("stone.png"), sf::Vector2f(1900, 1121), 0.f)));
	//manager.Append(std::dynamic_pointer_cast<GameObject>(std::make_shared<Stone>(TextureManager::Get("stone.png"), sf::Vector2f(1453, 1423), 0.f)));
	//manager.Append(Weapon(, , , ,));
	avalibleWeapons.resize(2);

	//std::shared_ptr<Crate> crate = std::make_shared<Crate>(&manager, TextureManager::Get("box.jpg"), sf::Vector2f(0, 0), 0);
	//manager.Append(std::dynamic_pointer_cast<GameObject>(crate));

	sf::View view(player->GetPos(), sf::Vector2f(1000, 1000));
	window.setView(view);

#ifdef CLIENT
	Packet packet(PacketType::PT_Create);
	packet.Append(player.get(), sizeof(Player));
	client.SendAll(packet.buffer.data(), packet.buffer.size());
#endif 
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
		window.setView(view);
		manager.UpdateAll();

#ifdef CLIENT
		Packet packet(PacketType::PT_Move);
		packet.Append(player.get(), sizeof(Player));
		client.SendAll(packet.buffer.data(), packet.buffer.size());

		client.ReceiveAll(buffer.data(), buffer.size());
#endif

#ifdef SERVER
		server.Frame();
#endif 
		//mySocket->SendAll();
#ifdef CLIENT
		window.clear();
		view.setCenter(player->GetPos());
		//window.draw(shape);
		manager.DrawAll();
		window.display();
#endif
	}
	return 0;
}