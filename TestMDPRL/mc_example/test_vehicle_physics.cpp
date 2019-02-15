//
//  test_vehicle_physics.cpp
//  TestMDPRL
//
//  Created by Henry Tominaga on 12/30/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "catch.hpp"
#include "vehicle_physics.hpp"

TEST_CASE("Test Position Move w Velocity but no Accel") {
    Position pos = Position(1, 2);
    Velocity v = Velocity(2, 3);
    Accel a = Accel(0, 0);
    Position new_pos = VehicleMoveWithAccel(pos, v, a);
    REQUIRE(new_pos.x == 3);
    REQUIRE(new_pos.y == 5);
}

TEST_CASE("Test Position Move w Velocity and Accel") {
    Position pos = Position(0, 1);
    Velocity v = Velocity(1, 0);
    Accel a = Accel(3, 4);
    Position new_pos = VehicleMoveWithAccel(pos, v, a);
    REQUIRE(new_pos.x == 4);
    REQUIRE(new_pos.y == 5);
}

TEST_CASE("Test Position Move w Velocity and Accel but limited by max velocity") {
    Position pos = Position(1, 2);
    Velocity v = Velocity(2, 3);
    Accel a = Accel(4, 5);
    Position new_pos = VehicleMoveWithAccel(pos, v, a);
    REQUIRE(new_pos.x == 6);
    REQUIRE(new_pos.y == 7);
}
