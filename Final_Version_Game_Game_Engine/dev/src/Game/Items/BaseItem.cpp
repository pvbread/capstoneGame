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

std::string BaseItem::getMessage() const
{
    std::string msg = "+" + std::to_string(itemMod) + " to ";
    if (itemType == "armor")
        msg += "armor";
    else if (itemType == "speed")
        msg += "speed";
    else if (itemType == "dodge")
        msg += "dodge";
    else if (itemType == "hit")
        msg += "hit";
    else
        return "No item";
    return msg;
}