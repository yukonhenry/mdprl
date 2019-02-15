//
//  config.hpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#include <iostream>
#include <vector>
struct Config {
    double simulation_step_size;
    double simulation_final_time;
};

Config getConfig(int argc, const char* argv[]);

std::vector<std::pair<int, int>> ReadDepotConfig(const char*);

std::vector<std::vector<int>> ReadRaceTrackConfig(const char*);

std::vector<std::pair<int, int>> ReadGridWorldConfig(const char*);

#endif /* config_hpp */
