#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{
    inventory.iron    = 1000;
    inventory.stone   = 1000;
    inventory.wood    = 1000;
    inventory.food    = 1000;
}

void Player::takeResources(Resources resources)
{
    inventory.wood -= resources.wood;
    inventory.stone-= resources.stone;
    inventory.iron -= resources.iron;
    inventory.food -= resources.food;
}

void Player::giveResources(Resources resources)
{
    inventory.wood += resources.wood;
    inventory.stone+= resources.stone;
    inventory.iron += resources.iron;
    inventory.food += resources.food;
}

void Player::checkState()
{
    if(inventory.iron < 0 || inventory.stone < 0 || inventory.wood < 0 || inventory.food < 0)
        emit playerLost(username);
}
