//
//  simulator.hpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef simulator_hpp
#define simulator_hpp
#include <iostream>

class Simulator {
public:
    Simulator(double step_size, double final_time): time_step(step_size), final_time(final_time) {};
    void TimeLoop();
private:
    double time_step, final_time;
};
#endif /* simulator_hpp */
