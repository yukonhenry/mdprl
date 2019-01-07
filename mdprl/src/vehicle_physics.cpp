//
//  racetrack_physics.cpp
//  mdprl
//
//  Created by Henry Tominaga on 11/4/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "vehicle_physics.hpp"
#include <algorithm>

Position VehicleMove(const Position& position, const Velocity& velocity) {
    Position new_position = Position(position.x + velocity.x, position.y + velocity.y);
    return new_position;
}

int BoundVelocity(int raw) {
    return std::min(kMaximumVelocity, std::max(kMinimumVelocity, raw));
}

Velocity VehicleAccel(const Velocity& velocity, const Accel& accel) {
    int new_velocity_x = BoundVelocity(velocity.x + accel.x);
    int new_velocity_y = BoundVelocity(velocity.y + accel.y);
    return Velocity(new_velocity_x, new_velocity_y);
}

Position VehicleMoveWithAccel(const Position& position, const Velocity& velocity, const Accel& accel) {
    Velocity new_velocity = VehicleAccel(velocity, accel);
    return VehicleMove(position, new_velocity);
}
