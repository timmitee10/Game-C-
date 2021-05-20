#pragma once
#include "GameObject.h"
#include "Weapon.h"
#include "Bullet.h"

#include  <random>
#include  <iterator>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}

/* Create a vector an amount of random weapons chosen from the weapon details vector*/
inline std::vector<Weapon*> randomWeapon(int count, std::vector<WeaponDetails>* weaponDetails, GameObjectManager* gameObjectManager, Character* target)
{
	std::vector<Weapon*> weapons;
	weapons.resize(count);
	/* Lambda function */
	std::for_each(weapons.begin(), weapons.end(), [&](Weapon* p) {
		auto a = select_randomly(weaponDetails->begin(), weaponDetails->end());
		p = new Weapon(gameObjectManager, target,(WeaponDetails*)&a);
	});
}

class Crate final : GameObject
{
public:
	Crate(GameObjectManager* gameObjects, const sf::Texture* texture, const sf::Vector2f& pos, float rotation,
		const sf::Vector2f& scale = sf::Vector2f(1, 1));
	virtual ~Crate();
	void Destroy();
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow* const renderer) const override;
private:
	std::vector<WeaponObject> weapons;
	bool isDestroyed = false;
};
