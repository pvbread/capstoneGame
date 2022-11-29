#pragma once
#include "pch.h"

class BaseItem
{
public:
    BaseItem() = default;
    BaseItem(std::string name, std::string type, int mod);
    std::string getName() const;
    std::string getType() const;
    int getMod() const;
    void setName(std::string name);
    void setType(std::string type);
    void setMod(int mod);

private:
    std::string itemName;
    std::string itemType;
    int itemMod;
};