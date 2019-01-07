//
//  simulator.cpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "simulator.hpp"

void Simulator::TimeLoop() {
    double current_time = 0.0;
    bool infinite_loop = final_time < 0.0 ? true : false;
    while (infinite_loop || current_time <= final_time) {
        std::cout << "time:" << current_time << std::endl;
        current_time += time_step;
    }
}
