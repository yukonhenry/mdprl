//
//  racetrack_learn.cpp
//  mdprl
//
//  Created by Henry Tominaga on 10/14/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include "racetrack_operations.hpp"
#include "vehicle_physics.hpp"
#include "config.hpp"
RaceTrackOperations::RaceTrackOperations(const char* filename) {
    racetrack_grid = ReadRaceTrackConfig(filename);
    racetrack_height = (int)racetrack_grid.size();
    racetrack_width = (int)racetrack_grid.at(0).size();
    std::vector<int> starting_row = racetrack_grid.at(0);
    number_starting_positions = 0;
    for (int index = 0; index < starting_row.size(); ++index) {
        if (starting_row.at(index) == 1) {
            ++number_starting_positions;
            starting_row_indices.push_back(index);
        }
    }
}

Position RaceTrackOperations::RandomizeStartLocation() {
    /*
    int number_starting_positions = (int) std::count(starting_row.begin(), starting_row.end(), 1);
    int index = 0;
    while (starting_row.at(index) != 1)
        ++index;
    int starting_index = index + rand() % number_starting_positions;
     */
    int randomized_start_index = rand() % number_starting_positions;
    int starting_index = starting_row_indices.at(randomized_start_index);
    Position start_pos = Position(starting_index, 0);
    std::map<Position, int>::iterator it = start_position_history.find(start_pos);
    if (it != start_position_history.end()) {
        it->second += 1;
    } else {
        start_position_history.insert(std::make_pair(start_pos, 1));
    }
    return start_pos;
}

void RaceTrackOperations::DumpStartPositionHistory() {
    std::cout << "start position history:" << std::endl;
    for (auto it = start_position_history.begin(); it != start_position_history.end(); ++it) {
        std::cout << "position:" << it->first.x << " " << it->first.y << " count:" << it->second << std::endl;
    }
}
int RaceTrackOperations::racetrack_grid_value(const Position& xy_pos) {
    return racetrack_grid.at(xy_pos.y).at(xy_pos.x);
}

TrackPositionStatus RaceTrackOperations::mapPositionToGridStatus(const Position& position) {
    TrackPositionStatus status;
    switch (racetrack_grid_value(position)) {
        case 0:
            status = TrackPositionStatus::OFF_TRACK;
            break;
        case 1:
            status = TrackPositionStatus::ON_TRACK;
            break;
        case 2:
            status = TrackPositionStatus::GOAL;
            break;
        default:
            throw std::logic_error("racetrack grid has invalid values");
            break;
    }
    return status;
}

TrackPositionStatus RaceTrackOperations::detectTrackPositionStatus(const Position& new_position,
                                                                   const Position& old_position,
                                                                   const Velocity& velocity) {
    TrackPositionStatus status = TrackPositionStatus::UNDEFINED;
    if (new_position.x < 0 || new_position.y < 0)
        status = TrackPositionStatus::OFF_TRACK;
    else if (new_position.x >= racetrack_width || new_position.y >= racetrack_height) {
        for (unsigned v_x = 0; v_x <= velocity.x; ++v_x) {
            for (unsigned v_y = 0; v_y <= velocity.y; ++v_y) {
                Position position = VehicleMove(old_position, Velocity(v_x, v_y));
                if (position.x >= racetrack_width || position.y >= racetrack_height) {
                    status = TrackPositionStatus::OFF_TRACK;
                    continue;
                }
                status = mapPositionToGridStatus(position);
                if (status == TrackPositionStatus::GOAL)
                    break;
                else
                    continue;
            }
            if (status == TrackPositionStatus::GOAL)
                break;
            else
                continue;
        }
    } else {
        status = mapPositionToGridStatus(new_position);
    }
    assert(status != TrackPositionStatus::UNDEFINED);
    return status;
}
