#include "Game/Items/BaseItem.h"

BaseItem::BaseItem(std::string name, std::string type, int mod)
{
    itemName = name;
    itemType = type;
    itemMod = mod;
}

std::string BaseItem::getName() const
{
    return itemName;
}

std::string BaseItem::getType() const
{
    return itemType;
}

int BaseItem::getMod() const
{
    return itemMod;
}

void BaseItem::setName(std::string name)
{
    itemName = name;
}

void BaseItem::setType(std::string type)
{
    itemType = type;
}

void BaseItem::setMod(int mod)
{
    itemMod = mod;
}