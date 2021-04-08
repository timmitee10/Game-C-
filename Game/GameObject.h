#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <math.h>
#define PI 3.14159265358979323846 
inline float RadToDeg(float radians)
{
	return (radians * 180) / PI;
}

inline float DegToRad(float degrees)
{
	return (degrees * PI) / 180;
}

inline sf::Vector2f normalize(const sf::Vector2f& source)
{
	const float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return {source.x / length, source.y / length};
	else
		return source;
}

inline sf::Vector2f FindDirection(const sf::Vector2f& source, const sf::Vector2f& target)
{
	const sf::Vector2f direction = target - source;
	normalize(direction);
	return direction;
}

class GameObject
{
public:
	GameObject(const sf::Texture* texture, const sf::Vector2f& pos, float rotation, const sf::Color& color,
		const sf::Vector2f& scale = sf::Vector2f(1, 1));
	virtual ~GameObject() {};
public:
	sf::Vector2f GetPos() const;
	float GetRotation() const;
	sf::Vector2f GetOrigin() const;
	sf::Vector2f GetScale() const;
	sf::Color GetColor() const;
	const sf::Texture* GetTexture() const;

	void SetPosition(const sf::Vector2f& pos);
	void SetRotation(const float rotation);
	void SetColor(const sf::Color& color);

	virtual void Update(float deltaTime);

	virtual void Draw(sf::RenderWindow* const renderer) const;

	bool Intersects(const sf::Rect<float>& rect) const;

	bool IntersectsTop(const sf::Rect<float>& rect) const;

	bool IntersectsBottom(const sf::Rect<float>& rect) const;

	bool IntersectsLeft(const sf::Rect<float>& rect) const;

	bool IntersectsRight(const sf::Rect<float>& rect) const;

protected:
	sf::Vector2f position;
	sf::Vector2f direction = sf::Vector2f(0, 0);
	sf::Vector2f origin;
	sf::Vector2f scale;
	sf::Color color;
	/* Rotation!! NOT radians!!!)(#/)(¤(#/)")¤/#"*/
	float rotation;
	const sf::Texture* texture;
	sf::Sprite sprite;
	sf::Rect<float> hitBox;
};
