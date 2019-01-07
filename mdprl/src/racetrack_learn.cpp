//
//  racetrack_learn.cpp
//  mdprl
//
//  Created by Henry Tominaga on 12/30/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//
#include <algorithm>
#include "racetrack_learn.hpp"

void RaceTrackLearn::OffPolicyControl() {
    //int best_g_reward = INT_MIN;
    int g_reward = INT_MIN;
    //int count = 0;
    int episode_counter = 0;
    std::map<Position, int> best_return_by_position;
    std::map<int, int> counts;
    counts.insert({vehicle.position.x, 0});
    while (std::any_of(counts.begin(), counts.end(), [](std::map<int, int>::const_reference item) {return item.second < 10;})) {
        Position start_position = Position(vehicle.position.x, vehicle.position.y);
        auto results = policy_model.ExecuteProcessEpisode(track_operations, vehicle);
        ++episode_counter;
        //std::cout << "episode counter:" << episode_counter << " Greward:" << results.second << std::endl;
        bool episode_match_flag = results.first;
        g_reward = results.second;
        if (episode_match_flag) {
            std::cout << "@@@@@ Full Episode @@ episode counte:" << episode_counter << " return" <<
                results.second << std::endl;
            std::map<Position, int>::iterator it = best_return_by_position.find(start_position);
            if (it == best_return_by_position.end()) {
                counts.insert({start_position.x, 0});
                best_return_by_position.insert({start_position, g_reward});
            } else {
                if (g_reward > it->second) {
                    counts[start_position.x] = 0;
                    it->second = g_reward;
                    std::cout << "---best_g_return updated for Positiom:" << it->first.x << " " << it->first.y << " " <<
                    g_reward << std::endl;
                } else {
                    counts[start_position.x] += 1;
                    std::cout << "**Count:" << counts.at(start_position.x) << " for position:" << start_position.x <<
                    std::endl;
                }
            }
            /*
            if (g_reward > best_g_reward) {
                best_g_reward = g_reward;
                count = 0;
                std::cout << "--best_g_reward updated:" << best_g_reward << std::endl;
            } else {
                ++count;
                std::cout << "^^^g_reward:" << g_reward << " best:" << best_g_reward << " count:" << count << std::endl;
            } */
        } else {
            //std::cout << "&& Partial Episode Match" << std::endl;
        }
        vehicle.ResetVehicle(track_operations.RandomizeStartLocation());
    }
    policy_model.DumpPolicy(track_operations);
    track_operations.DumpStartPositionHistory();
    std::cout << "best returns by position" << std::endl;
    for (auto it = best_return_by_position.begin(); it != best_return_by_position.end(); ++it) {
        std::cout << "position:" << it->first.x << " " << it->first.y << " return:" << it->second << std::endl;
    }
    std::cout << "full episode counts by position" << std::endl;
    for (auto it = counts.begin(); it != counts.end(); ++it) {
        std::cout << "position:" << it->first << " full episodes:" << it->second << std::endl;
    }


}
