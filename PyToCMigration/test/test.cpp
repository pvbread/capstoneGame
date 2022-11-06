#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Characters/Players/BasePlayer.h"

TEST_CASE("Check that stats are set for BasePlayer properly") 
{
    //name, Hp, speed, hit, armor, itemMod, speedMod, dodgeMod
    BasePlayer baseP = BasePlayer("BaseP", 30, 1, 3, 0, 4, 5, 6);

    SECTION("Check Name") 
    {
        REQUIRE(baseP.getName() == "BaseP");
    }
    SECTION("Check life")
    {
        REQUIRE(baseP.isAlive() == true);
    }
    SECTION("Is enemy?")
    {
        REQUIRE(baseP.isEnemy() == false); 
    }
    SECTION("Check HP")
    {
        REQUIRE(baseP.getHp() == 30);
    }
    SECTION("Check maxHp")
    {
        REQUIRE(baseP.getMaxHp() == 30);
    }
    SECTION("Check speed")
    {
        REQUIRE(baseP.getSpeed() == 1);
    }
    SECTION("Check hit")
    {
        REQUIRE(baseP.getHit() == 3);
    }
    SECTION("Check armor")
    {
        REQUIRE(baseP.getArmor() == 0);
    }
    SECTION("Check itemMod")
    {
        REQUIRE(baseP.getItemModifier() == 4);
    }
    SECTION("Check speedMod")
    {
        REQUIRE(baseP.getSpeedModifier() == 5);
    }
    SECTION("Check dodgeMod")
    {
        REQUIRE(baseP.getDodgeModifier() == 6);
    }
}

TEST_CASE("Check that stats can be manually set") 
{
    //name, Hp, speed, hit, armor, itemMod, speedMod, dodgeMod
    BasePlayer baseP = BasePlayer("BaseP", 30, 1, 3, 0, 4, 5, 6);

    SECTION("Check set Name") 
    {
        baseP.setName("Foo");
        REQUIRE(baseP.getName() == "Foo");
    }
    SECTION("Check set Index")
    {
        baseP.setNewParticipantsIndex(4);
        REQUIRE(baseP.getParticipantsIndex() == 4);
    }
    SECTION("Check set life")
    {
        baseP.changeLifeStatus(false);
        REQUIRE(baseP.isAlive() == false);
    }
    SECTION("Is enemy? set")
    {
        baseP.setAsPlayerOrEnemy(true);
        REQUIRE(baseP.isEnemy() == true); 
    }
    SECTION("Check set HP")
    {
        baseP.setHp(23);
        REQUIRE(baseP.getHp() == 23);
    }
    SECTION("Check set maxHp")
    {
        baseP.setMaxHp(3000);
        REQUIRE(baseP.getMaxHp() == 3000);
    }
    SECTION("Check set speed")
    {
        baseP.setSpeed(6);
        REQUIRE(baseP.getSpeed() == 6);
    }
    SECTION("Check set hit")
    {
        baseP.setHit(4);
        REQUIRE(baseP.getHit() == 4);
    }
    SECTION("Check set armor")
    {
        baseP.setArmor(12);
        REQUIRE(baseP.getArmor() == 12);
    }
    SECTION("Check set itemMod")
    {
        baseP.setItemModifier(15);
        REQUIRE(baseP.getItemModifier() == 15);
    }
    SECTION("Check set speedMod")
    {
        baseP.setSpeedModifier(2);
        REQUIRE(baseP.getSpeedModifier() == 2);
    }
    SECTION("Check set dodgeMod")
    {
        baseP.setDodgeModifier(3);
        REQUIRE(baseP.getDodgeModifier() == 3);
    }
}