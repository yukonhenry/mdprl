//
//  racetrack_vehicle.cpp
//  mdprl
//
//  Created by Henry Tominaga on 10/21/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "racetrack_vehicle.hpp"
#include "vehicle_physics.hpp"
void RaceTrackVehicle::ResetVehicle(Position new_position) {
    position.x = new_position.x;
    position.y = new_position.y;
    velocity.x = 0;
    velocity.y = 0;
    position_status = TrackPositionStatus::ON_TRACK;
}

void RaceTrackVehicle::UpdateVehicleWithAccelAction(Accel& accel) {
    old_velocity = velocity;
    velocity = VehicleAccel(velocity, accel);
    old_position = position;
    position = VehicleMove(position, velocity);
}
