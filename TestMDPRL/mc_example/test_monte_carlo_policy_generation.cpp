//
//  test_monte_carlo_policy_generation.cpp
//  TestMDPRL
//
//  Created by Henry Tominaga on 12/28/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "catch.hpp"
#include "monte_carlo_policy_model.hpp"

TEST_CASE("Create Policy Model") {
    RaceTrackOperations operations = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    MonteCarloPolicyModel model = MonteCarloPolicyModel(operations.racetrack_height, operations.racetrack_width, 5);
    REQUIRE(true);
}


TEST_CASE("Test Random Policy Generation") {
    const int max_positive_action = 10;
    MonteCarloPolicyModel model = MonteCarloPolicyModel(3, 5, max_positive_action);
    RaceTrackOperations operations = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    RaceTrackVehicle vehicle = RaceTrackVehicle(Position(0, 0));
    auto executed = model.ExecuteBPolicy(operations, vehicle);
    Accel executed_accel = executed.first;
    REQUIRE(executed_accel.x <= max_positive_action);
    REQUIRE(executed_accel.x >= -max_positive_action);
    REQUIRE(executed_accel.y <= max_positive_action);
    REQUIRE(executed_accel.y >= -max_positive_action);
}
