///Users/htominaga/workspace/mdprl/TestMDPRL
//  test_racetrack_learn.cpp
//  mdprl
//
//  Created by Henry Tominaga on 10/21/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "catch.hpp"
#include "racetrack_operations.hpp"
#include "racetrack_vehicle.hpp"

TEST_CASE("Confirm start location generated correctly") {
    RaceTrackOperations operations = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    RaceTrackVehicle vehicle = RaceTrackVehicle(operations.RandomizeStartLocation());
    REQUIRE(vehicle.velocity.x == 0);
    REQUIRE(vehicle.velocity.y == 0);
}

TEST_CASE("Confirm Racetrack position detector detects positions that are outside of track") {
    RaceTrackOperations rtl = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    Velocity v = Velocity(0, 0);
    Position old_pos = Position(0, 0);
    REQUIRE(rtl.detectTrackPositionStatus(Position(-1, 0), old_pos, v) == TrackPositionStatus::OFF_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(5, 3), old_pos, v) == TrackPositionStatus::OFF_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(3, 5), old_pos, v) == TrackPositionStatus::OFF_TRACK);
}

TEST_CASE("Confirm Racetrack position detector detects positions that hit obstacle") {
    RaceTrackOperations rtl = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    Velocity v = Velocity(0, 0);
    Position old_pos = Position(0, 0);
    REQUIRE(rtl.detectTrackPositionStatus(Position(0, 0), old_pos, v) == TrackPositionStatus::OFF_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(0, 1), old_pos, v) == TrackPositionStatus::OFF_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(4, 0), old_pos, v) == TrackPositionStatus::OFF_TRACK);
}

TEST_CASE("Confirm Racetrack position detector detects positions on track") {
    RaceTrackOperations rtl = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    Velocity v = Velocity(0, 0);
    Position old_pos = Position(0, 0);
    REQUIRE(rtl.detectTrackPositionStatus(Position(1, 0), old_pos, v) == TrackPositionStatus::ON_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(1, 1), old_pos, v) == TrackPositionStatus::ON_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(2, 2), old_pos, v) == TrackPositionStatus::ON_TRACK);
    REQUIRE(rtl.detectTrackPositionStatus(Position(3, 2), old_pos, v) == TrackPositionStatus::ON_TRACK);
}

TEST_CASE("Confirm Racetrack position detector detects goal or passing of goal") {
    RaceTrackOperations rtl = RaceTrackOperations("mc_example/racetrack_config_small.txt");
    Velocity v0 = Velocity(0, 0);
    Position old_pos = Position(0, 0);
    REQUIRE(rtl.detectTrackPositionStatus(Position(4, 1), old_pos, v0) == TrackPositionStatus::GOAL);
    REQUIRE(rtl.detectTrackPositionStatus(Position(4, 2), old_pos, v0) == TrackPositionStatus::GOAL);
    Velocity v1 = Velocity(5, 0);
    Position p1 = Position(1, 1);
    Position p2 = Position(1, 2);
    REQUIRE(rtl.detectTrackPositionStatus(Position(0, 5), p1, v1) == TrackPositionStatus::GOAL);
    REQUIRE(rtl.detectTrackPositionStatus(Position(5, 1), p2, v1) == TrackPositionStatus::GOAL);
}

