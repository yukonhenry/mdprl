//
//  test_grid_world_create.cpp
//  TestMDPRL
//
//  Created by Henry Tominaga on 1/13/19.
//  Copyright © 2019 Henry Tominaga. All rights reserved.
//
#include "catch.hpp"
#include "config.hpp"
#include "grid_world.hpp"

TEST_CASE("Create grid world") {
    auto params = ReadGridWorldConfig("td0_example/grid_world_config.txt");
    GridWorld world = GridWorld(params);
    REQUIRE(world.grid.size() == 6);
    REQUIRE(world.grid.at(0).size() == 5);
}

