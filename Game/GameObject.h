#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <math.h>
#define PI 3.14159265358979323846 
class GameObjectManager;

inline float RadToDeg(float radians)
{
	return (radians * 180.f) / PI;
}

inline float DegToRad(float degrees)
{
	return (2 * PI * (degrees / 360));
	//return (degrees * PI) / 180.f;
}

inline sf::Vector2f normalize(const sf::Vector2f& source)
{
	const float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return { source.x / length, source.y / length };
	else
		return source;
}

inline sf::Vector2f FindDirection(const sf::Vector2f& source, const sf::Vector2f& target)
{
	const sf::Vector2f direction = target - source;
	normalize(direction);
	return direction;
}

inline sf::Vector2f calcDirection(float degRotation)
{
	return sf::Vector2f(std::cos(DegToRad(90) - DegToRad(degRotation)), -std::sin(DegToRad(90) - DegToRad(degRotation)));
}

inline float calcRotation(const sf::Vector2f& direction)
{
	return std::atan2(direction.y, direction.x);
}
template<typename T>
inline T Vector2Distance(const sf::Vector2<T>& from, const sf::Vector2<T>& to) 
{
	auto result = from - to;
	return std::sqrt(result.x * result.x + result.y * result.y);
}
template<typename Base, typename Derived>
struct PolymorphicComparable : public Base {

	bool operator==(const Base& other) const {
		// first check if the dynamic types match
		if (typeid(other) != typeid(Derived))
			return false;

		// cast to the concrete types; thanks to the check above this is safe
		const Derived& a = static_cast<const Derived&>(*this);
		const Derived& b = static_cast<const Derived&>(other);

		// redirect to Derived::operator==(Derived)
		return a == b;
	}
};

class BaseGameObject
{
	virtual bool operator==(const BaseGameObject& lhs) = 0;
};

class GameObject
{
public:
	GameObject(const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1));

	GameObject(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1));

	virtual ~GameObject();
	using HitBox = sf::Rect<int>;
public:
	void SetRelativePositionAndRotation(GameObject* target, sf::Vector2f offset);
	sf::Vector2f& GetPos();
	float GetRotation();
	sf::Vector2f GetOrigin() const;
	sf::Vector2f GetScale() const;
	sf::Color GetColor() const;
	HitBox* GetHitBox() const;
	const sf::Texture* GetTexture() const;
	bool IsRemoved() const;
	void SetPosition(const sf::Vector2f& pos);
	void SetRotation(const float rotation);
	void SetColor(const sf::Color& color);

	virtual void Update(float deltaTime);

	virtual void Draw(sf::RenderWindow* const renderer) const;

	bool Intersects(GameObject::HitBox* rect) const;

	bool IntersectsTop(GameObject::HitBox* rect) const;

	bool IntersectsBottom(GameObject::HitBox* rect) const;

	bool IntersectsLeft(GameObject::HitBox* rect) const;

	bool IntersectsRight(GameObject::HitBox* rect) const;

	virtual void OnCollision(GameObject* object);

	sf::Sprite CloneSprite() const;

	void AddForce(sf::Vector2f direction, float velocity, float deltaTime);
	void AddForce(float rotation, float velocity, float deltaTime);

	uint64_t GetId() const;
protected:
	sf::Vector2f position;
	sf::Vector2f direction = sf::Vector2f(0, 0);
	sf::Vector2f origin;
	sf::Vector2f scale;
	sf::Color color;
	/* Rotation in degrease */
	float rotation = 0;
	const sf::Texture* texture;
	sf::Sprite sprite;
	HitBox* hitBox = nullptr;
	GameObjectManager* objectManager;
	bool isRemoved = false;

	uint64_t uid;
};