#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Characters/Players/BasePlayer.h"
#include "Characters/BaseCharacter.h"
#include <vector>

TEST_CASE("Test attack method")
{
    BasePlayer p1 = BasePlayer("p1", 30, 1, 3, 0, 4, 5, 6);
    BasePlayer p2 = BasePlayer("p2", 30, 1, 3, 0, 4, 5, 6);
    BasePlayer pNoDodgeNoArmor = BasePlayer("p2", 30, 1, 3, 0, 4, 5, 0);
    BasePlayer incredibleArmor = BasePlayer("p2", 30, 1, 3, INT_MAX, 4, 5, 0);   
    SECTION("Check attack returns a positive int")
    {
        REQUIRE(p1.attack(p2) >= 0);
    }
    SECTION("Check attack against 0 dodge 0 armor is successful")
    {
        int minAttack = INT_MAX;
        for (int i = 0; i < 1000; i++)
        {
            minAttack = std::min(minAttack, p1.attack(pNoDodgeNoArmor));
        }
        REQUIRE(minAttack > 0);
    }
    SECTION("Check attack GOD armor is not sucessful")
    {
        int maxAttack = INT_MIN;
        for (int i = 0; i < 1000; i++)
        {
            maxAttack = std::max(maxAttack, p1.attack(incredibleArmor));
        }
        REQUIRE(maxAttack == 0);
    }
}

TEST_CASE("Test buff")
{
    BasePlayer p1 = BasePlayer("p1", 30, 1, 3, 0, 4, 5, 6);
    BasePlayer p2 = BasePlayer("p2", 30, 1, 3, 0, 4, 5, 6);
    BasePlayer dead = BasePlayer("p1", 0, 1, 3, 0, 4, 5, 6);
    dead.changeLifeStatus(false);
    BasePlayer loweredHp = BasePlayer("p2", 30, 1, 3, 0, 4, 5, 6);
    loweredHp.setHp(20); //lowers it below its max

    SECTION("Healing a fully healed char")
    {
        int oldHp = p2.getHp();
        REQUIRE(p1.buff(p2) == oldHp);
    }
    SECTION("Healing a dead char")
    {
        REQUIRE(p1.buff(dead) == 0);
    }
    SECTION("Healing a wounded char")
    {
        int oldHp = loweredHp.getHp();
        REQUIRE(p1.buff(loweredHp) > oldHp);
    }
}

TEST_CASE("Test debuff")
{
    BasePlayer p1 = BasePlayer("p1", 30, 1, 3, 0, 4, 5, 6);
    BasePlayer zeroSpeed = BasePlayer("p2", 30, 1, 3, 0, 4, 0, 6);
    BasePlayer p2 = BasePlayer("p2", 30, 1, 3, 0, 4, 2, 6);

    //SECTION("Debuffing a player with 0 speedMod")  
    SECTION("Testing to check effect of normal debuff")
    {
        int oldSpeedMod = p2.getSpeedModifier();
        REQUIRE(p1.debuff(p2) < oldSpeedMod);
    }
}

TEST_CASE("Test moveSpots")
{
    BasePlayer p1 = BasePlayer("p1", 30, 1, 3, 0, 4, 5, 6);
    BaseCharacter p2 = BaseCharacter("p2", 30, 1, 3, 0, 4, 0, 6, false);

    std::vector<BaseCharacter> v{p1, p2};

    SECTION("Check that swap with self has no effect")
    {
        std::vector<BaseCharacter> res = p1.moveSpots(0, 0, v);
        bool noSwap = res[0].getName() == "p1" && res[1].getName() == "p2";
        REQUIRE(noSwap);
    }
    SECTION("Check that normal swap works")
    { 
        std::vector<BaseCharacter> res = p1.moveSpots(0, 1, v);
        bool swapped = res[0].getName() == "p2" && res[1].getName() == "p1";
        REQUIRE(swapped);
    }
    //TODO check that you can't swap with enemy
}

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