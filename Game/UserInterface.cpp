#include "UserInterface.h"

UserInterface::UserInterface(Character* target, const sf::Texture* uiRifle, const sf::Texture* uiPistol, const sf::Texture* uiBandage) :
	target(target), inventory(target->GetInventory())
{
	target->GetHealth();
}

UserInterface::~UserInterface()
{

}

void UserInterface::Draw(sf::RenderWindow* render)
{
	healthBar->Draw(render);
}
