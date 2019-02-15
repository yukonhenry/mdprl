//
//  racetrack_operations.hpp
//  mdprl
//
//  Created by Henry Tominaga on 10/14/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef racetrack_operations_hpp
#define racetrack_operations_hpp

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "racetrack_vehicle.hpp"
#include "racetrack_util.hpp"
class RaceTrackOperations {
public:
    RaceTrackOperations(const char*);
    TrackPositionStatus detectTrackPositionStatus(const Position& new_position, const Position& old_position,
                                                  const Velocity& v);
    Position RandomizeStartLocation();
    int racetrack_height, racetrack_width;
    void DumpStartPositionHistory();
    std::vector<int> starting_row_indices;
private:
    std::vector<std::vector<int>> racetrack_grid;
    int racetrack_grid_value(const Position&);
    TrackPositionStatus mapPositionToGridStatus(const Position&);
    std::map<Position, int> start_position_history;
    int number_starting_positions;
};
#endif /* racetrack_operations_hpp */
