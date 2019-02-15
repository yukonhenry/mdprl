//
//  grid_world.cpp
//  mdprl
//
//  Created by Henry Tominaga on 1/13/19.
//  Copyright © 2019 Henry Tominaga. All rights reserved.
//

#include "grid_world.hpp"

GridWorld::GridWorld(std::vector<std::pair<int, int>>& configs):
    grid(configs.at(0).first, std::vector<int>(configs.at(0).second)),
    start(Position(configs.at(1).first, configs.at(1).second)),
    goal(Position(configs.at(2).first, configs.at(2).second)),
    winds(configs.at(0).first) {
        grid.at(start.x).at(start.y) = 2;
        grid.at(goal.x).at(goal.y) = 3;
        for (int i = 3; i < configs.size(); ++i) {
            auto config_elem = configs.at(i);
            winds.at(config_elem.first) = config_elem.second;
        }
    };
