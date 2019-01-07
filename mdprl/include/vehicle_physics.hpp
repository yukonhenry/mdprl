//
//  vehicle_physics.hpp
//  mdprl
//
//  Created by Henry Tominaga on 11/4/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef vehicle_physics_hpp
#define vehicle_physics_hpp

#include <iostream>
#include "racetrack_vehicle.hpp"
const int kMinimumVelocity = 0;
const int kMaximumVelocity = 5;
Position VehicleMove(const Position&, const Velocity&);
Velocity VehicleAccel(const Velocity&, const Accel&);
Position VehicleMoveWithAccel(const Position&, const Velocity&, const Accel&);
int BoundVelocity(int);
#endif /* vehicle_physics_hpp */
