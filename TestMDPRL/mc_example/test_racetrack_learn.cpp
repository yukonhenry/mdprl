//
//  test_racetrack_learn.cpp
//  TestMDPRL
//
//  Created by Henry Tominaga on 12/31/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include <iostream>
#include "catch.hpp"
#include "racetrack_learn.hpp"

TEST_CASE("Instantiate Racetrack learn object and read back properties") {
    RaceTrackOperations operations = RaceTrackOperations("mc_example/racetrack_configLL.txt");
    RaceTrackLearn learn = RaceTrackLearn(operations);
    learn.OffPolicyControl();
    REQUIRE(true);
}
