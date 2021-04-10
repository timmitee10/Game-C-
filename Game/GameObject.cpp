#include "GameObject.h"

GameObject::GameObject(const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
	const sf::Vector2f& scale) :
	position(pos), origin(pos.x / 2, pos.y / 2), scale(scale), rotation(rotation), texture(texture)
{
	this->direction = calcDirection(rotation);
	sprite.setPosition(this->position);
	sprite.setOrigin(this->origin);
	sprite.setRotation(this->rotation);
	sprite.setTexture(*this->texture);
	sprite.setScale(this->scale);
	//sprite.setColor(this->color);
}
GameObject::GameObject(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
	const sf::Vector2f& scale) : GameObject(texture, pos, rotation, scale)
{
	this->objectManager = gameObjects;
}

void GameObject::SetRelativePositionAndRotation(GameObject* target, sf::Vector2f offset)
{
	this->position = target->position + offset;
	this->rotation = target->rotation;
}

sf::Vector2f& GameObject::GetPos()
{
	return position;
}

float& GameObject::GetRotation()
{
	return rotation;
}

sf::Vector2f GameObject::GetOrigin() const
{
	return origin;
}

sf::Vector2f GameObject::GetScale() const
{
	return scale;
}

sf::Color GameObject::GetColor() const
{
	return color;
}

const sf::Rect<float>& GameObject::GetRect() const
{
	return this->hitBox;
}

const sf::Texture* GameObject::GetTexture() const
{
	return texture;
}

bool GameObject::IsRemoved() const
{
	return isRemoved;
}

void GameObject::SetPosition(const sf::Vector2f& pos)
{
	this->position = pos;
}

void GameObject::SetRotation(const float rotation)
{
	this->rotation = rotation;
}

void GameObject::SetColor(const sf::Color& color)
{
	this->color = color;
}

void GameObject::Update(float deltaTime)
{
	//TODO fix direction calculation
	//myDirection = new Vector2((float)Math.Cos(MathHelper.ToRadians(90) - myRotation), -(float)Math.Sin(MathHelper.ToRadians(90) - myRotation));

	//direction = sf::Vector2f(std::cos(DegToRad(90) - rotation), -std::sin(DegToRad(90) - rotation));
	direction = calcDirection(rotation);
}

void GameObject::Draw(sf::RenderWindow* renderer) const
{
	renderer->draw(sprite);
}

bool GameObject::Intersects(const sf::Rect<float>& rect) const
{
	return this->hitBox.intersects(rect);
}

bool GameObject::IntersectsTop(const sf::Rect<float>& rect) const
{
	return false;
}

bool GameObject::IntersectsBottom(const sf::Rect<float>& rect) const
{
	return false;
}

bool GameObject::IntersectsLeft(const sf::Rect<float>& rect) const
{
	return false;
}

bool GameObject::IntersectsRight(const sf::Rect<float>& rect) const
{
	return false;
}

bool GameObject::operator==(const GameObject& lhs) const
{
	return *this == lhs;
}

bool GameObject::operator!=(const GameObject& lhs) const
{
	return !(this->operator==(lhs));
}

void GameObject::OnCollision(GameObject* object)
{
}
