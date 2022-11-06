#include <iostream>
#include "Characters/BaseCharacter.h"
#include "Characters/Players/Bass.h"
#include "Characters/Players/BasePlayer.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Characters/Players/BasePlayer.h"

TEST_CASE("FindMax") {
  BasePlayer baseP = BasePlayer("BaseP", 30, 1, 3, 0, 3, 3, 3);

  SECTION("Should find check basic stats of BasePlayer have been set correctly") {
    REQUIRE(baseP.getName() == "BaseP");
  }
}