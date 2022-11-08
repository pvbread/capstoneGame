#include "catch.hpp"

#include "Utility/isTeamAlive.h"
#include "Characters/Players/BasePlayer.h"
#include "Characters/Players/Bass.h"
#include "Characters/BaseCharacter.h"
#include <vector>


TEST_CASE("Test isTeamAlive()")
{
    BasePlayer p1 = BasePlayer("P1", 30, 1, 1, 0, 3, 3, 3);
    Bass p2 = Bass("PFast", 30, 6, 3, 0, 3, 3, 3);
    Bass p3 = Bass("PFastish", 30, 5, 3, 0, 3, 3, 3);
    Bass pDead = Bass("PSlow", 30, 1, 3, 0, 3, 3, 3);
    BaseCharacter e1 = BaseCharacter("eFast", 30, 6, 3, 0, 3, 3, 3, true);
    BaseCharacter e2 = BaseCharacter("eSlow", 30, 0, 3, 0, 3, 3, 3, true);
    BaseCharacter e3 = BaseCharacter("eMid", 30, 2, 3, 0, 3, 3, 3, true);
    BaseCharacter eDead = BaseCharacter("eMid2", 30, 3, 3, 0, 3, 3, 3, true);
    pDead.setHp(0);
    eDead.setHp(0);
    std::vector<BaseCharacter> participants{p1,p2,p3,pDead,e1,e2,e3,eDead};
    std::vector<BaseCharacter> deadAllies{pDead,pDead,pDead,pDead,e1,e2,e3,eDead};
    std::vector<BaseCharacter> deadEnemies{p1,p2,p3,pDead,eDead,eDead,eDead,eDead};
    SECTION("check for allies to be alive (should be )")
    {
        REQUIRE(isTeamAlive(participants, false) == true);
    }

    SECTION("check for enemies to be alive (should be)")
    {
        REQUIRE(isTeamAlive(participants, true) == true);
    }

    SECTION("check for allies to be dead (should be)")
    {
        REQUIRE(isTeamAlive(deadAllies, false) == false);
    }

    SECTION("check for enemies to be dead (should be)")
    {
        REQUIRE(isTeamAlive(deadEnemies, true) == false);
    }
}
