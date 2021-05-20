#include "Player.h"

Player::Player(float health, float velocity, GameObjectManager* gameObjects, const sf::Texture* texture,
	const sf::Vector2f& pos, float rotation, sf::RenderWindow* window, const sf::Vector2f& scale) : Character(
		health, velocity, gameObjects, texture, pos, rotation, scale), window(window)
{
}


void Player::Update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->AddForce(360, movementSpeed, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->AddForce(90, movementSpeed, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->AddForce(180 + 90, movementSpeed, deltaTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->AddForce(180, movementSpeed, deltaTime);
	}

	//gets sprite origin coordinates and mouse coordinates
	//auto mousePos = sf::Mouse::getPosition(*window);
	//auto mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));
	//auto mouseAngle = -atan2(mouse.x - position.x, mouse.y - position.y) * 180 / 3.14159; //angle in degrees of rotation for sprite

	//rotation = mouseAngle;
	//float dX = mousePos.x - position.x;
	//float dY = mousePos.y - position.y;

	//float rotation = atan2(dY, dX) * 180 / PI;

	sf::Vector2f curPos = this->position;
	sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*window));

	// now we have both the sprite position and the cursor
	// position lets do the calculation so our sprite will
	// face the position of the mouse

	float dx = curPos.x - mouse.x;
	float dy = curPos.y - mouse.y;

	this->rotation = (atan2(dy, dx)) * 180 / PI;

	sprite.setRotation(rotation);

	//this->sprite.setRotation(rotation);
	//this->direction = FindDirection(this->position, sf::Vector2f(mouse));
	//this->rotation = calcRotation(direction);
	//sf::Vector2f dir;
	//dir.x = sf::Mouse::getPosition().x - dir.x;
	//dir.y = sf::Mouse::getPosition().y - dir.y;

	//float rotatio = std::atan2f(dir.y, dir.x);

	//this->sprite.setRotation(rotation + 90.f);


	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		
	}

}

void Player::Draw(sf::RenderWindow* const renderer) const
{
	renderer->draw(this->sprite);
}