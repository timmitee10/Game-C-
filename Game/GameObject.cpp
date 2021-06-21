#include "GameObject.h"

GameObject::GameObject(const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
	const sf::Vector2f& scale) :
	position(pos), origin(texture->getSize().x / 2, texture->getSize().x / 2), scale(scale), rotation(rotation), texture(texture)
{
	this->direction = calcDirection(rotation);
	sprite.setPosition(this->position);
	sprite.setOrigin(this->origin);
	sprite.setRotation(this->rotation);
	sprite.setTexture(*this->texture);
	sprite.setScale(this->scale);
	this->hitBox = (HitBox*)&this->sprite.getTextureRect();
	this->direction = calcDirection(rotation);
	//sprite.setColor(this->color);
}
GameObject::GameObject(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
	const sf::Vector2f& scale) : GameObject(texture, pos, rotation, scale)
{
	this->objectManager = gameObjects;
}

GameObject::~GameObject()
{
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

float GameObject::GetRotation()
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

GameObject::HitBox* GameObject::GetHitBox() const
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
	this->sprite.setPosition(this->position);
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
	//direction = calcDirection(rotation);
}

void GameObject::Draw(sf::RenderWindow* renderer) const
{
	renderer->draw(sprite);
}

bool GameObject::Intersects(GameObject::HitBox* rect) const
{
	if (this->hitBox != nullptr && rect != nullptr)
		return this->hitBox->intersects(*rect);
}

bool GameObject::IntersectsTop(GameObject::HitBox* rect) const
{

	return false;
}

bool GameObject::IntersectsBottom(GameObject::HitBox* rect) const
{
	return false;
}

bool GameObject::IntersectsLeft(GameObject::HitBox* rect) const
{
	return false;
}

bool GameObject::IntersectsRight(GameObject::HitBox* rect) const
{
	return false;
}

//bool GameObject::operator==(const GameObject& lhs) const
//{
//	return *this == lhs;
//}

//bool GameObject::operator!=(const GameObject& lhs) const
//{
//	return !(this->operator==(lhs));
//}

void GameObject::OnCollision(GameObject* object)
{

}

sf::Sprite GameObject::CloneSprite() const
{
	return this->sprite;
}

void GameObject::AddForce(sf::Vector2f direction, float velocity, float deltaTime)
{
	this->position += direction * velocity * deltaTime * 0.0000001f;
}

void GameObject::AddForce(float rotation, float velocity, float deltaTime)
{
	this->position = this->sprite.getPosition();
	this->position += calcDirection(rotation) * velocity * deltaTime * 0.0000001f;
	this->sprite.setPosition(position);
	//this->SetPosition(this->position);
}

uint64_t GameObject::GetId() const
{
	return this->uid;
}

void GameObject::SetOwner(ObjectOwner owner)
{
	this.owner = owner;
}

ObjectOwner GameObject::GetOwner() const
{
	return this.owner;
}
