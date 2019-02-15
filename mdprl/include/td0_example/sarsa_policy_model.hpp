//
//  sarsa_policy_model.hpp
//  mdprl
//
//  Created by Henry Tominaga on 1/13/19.
//  Copyright © 2019 Henry Tominaga. All rights reserved.
//

#ifndef sarsa_policy_model_hpp
#define sarsa_policy_model_hpp

#include <iostream>
#include <random>
#include "grid_world.hpp"

class SarsaPolicyModel {
public:
    SarsaPolicyModel(const int width, const int height, const int max_action,
                     const Position&, const Position&);
    std::vector<
    std::vector<
    std::vector<double>>> state_action_q_matrix;
    void ExecuteEpisode();
private:
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    
    Position start;
    Position goal;
    int max_action;
    int FindBestAction(Position& state);
};
#endif /* sarsa_policy_model_hpp */
