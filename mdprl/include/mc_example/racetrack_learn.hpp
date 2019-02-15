//
//  racetrack_learn.hpp
//  mdprl
//
//  Created by Henry Tominaga on 12/30/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef racetrack_learn_hpp
#define racetrack_learn_hpp

#include <iostream>
#include "monte_carlo_policy_model.hpp"
#include "racetrack_operations.hpp"
#include "racetrack_vehicle.hpp"
const int kMaxPositiveAction = 1;
class RaceTrackLearn {
public:
    RaceTrackLearn(RaceTrackOperations& operations):track_operations(operations), vehicle(RaceTrackVehicle(operations.RandomizeStartLocation())),
    policy_model(MonteCarloPolicyModel(operations.racetrack_height, operations.racetrack_width, kMaxPositiveAction)) {}
    void OffPolicyControl();
private:
    RaceTrackOperations& track_operations;
    RaceTrackVehicle vehicle;
    MonteCarloPolicyModel policy_model;
};

#endif /* racetrack_learn_hpp */
