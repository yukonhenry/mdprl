//
//  main.cpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//
#include <cassert>
#include <iostream>
#include <unistd.h>
#include "config.hpp"
#include "policy_model.hpp"
#include "racetrack_learn.hpp"
#include "simulator.hpp"
#include "state_action_model.hpp"

int main(int argc, const char * argv[]) {
    Config config = getConfig(argc, argv);
    //RaceTrackLearn rtl = RaceTrackLearn();
    //auto init_position = rtl.RandomizeStartLocation();
    StateActionModel state_model = StateActionModel(20, 2, 10, 0.9);
    PolicyModel policy_model = PolicyModel(state_model.max_state_value);
    policy_model.PolicyIteration(state_model);
    return 0;
}
