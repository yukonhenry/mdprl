//
//  racetrack_vehicle.hpp
//  mdprl
//
//  Created by Henry Tominaga on 10/21/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef racetrack_vehicle_hpp
#define racetrack_vehicle_hpp

#include <iostream>
#include "racetrack_util.hpp"

struct Position {
    int x;
    int y;
    Position(int x, int y): x(x), y(y) {};
    Position(): x(0), y(0) {};
    // ref https://stackoverflow.com/questions/7204283/struct-as-a-key-in-a-stdmap
    bool const operator==(const Position &p) const {
        return x == p.x && y == p.y;
    }
    
    bool const operator<(const Position &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }};

struct Velocity {
    int x;
    int y;
    Velocity(int x, int y): x(x), y(y) {};
    Velocity(): x(0), y(0) {};
};

struct Accel {
    int x;
    int y;
    Accel(int x, int y): x(x), y(y) {};
    Accel(): x(0), y(0) {};
};

class RaceTrackVehicle {
public:
    RaceTrackVehicle(Position initial): velocity(0, 0),
    position(initial), old_position(initial), old_velocity(0, 0),
    position_status(TrackPositionStatus::ON_TRACK) {};
    Position old_position;
    Position position;
    Velocity old_velocity;
    Velocity velocity;
    TrackPositionStatus position_status;
    void UpdateVehicleWithAccelAction(Accel&);
    void ResetVehicle(Position);
};

#endif /* racetrack_vehicle_hpp */
