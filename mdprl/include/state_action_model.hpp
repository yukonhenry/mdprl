//
//  state_transition_model.hpp
//  mdprl
//
//  Created by Henry Tominaga on 9/8/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef state_action_model_hpp
#define state_action_model_hpp

#include <iostream>
#include <vector>
#include "poisson.hpp"

class StateActionModel {
public:
    StateActionModel(const int max_value, int, int, double);

    double ComputeStateValue(std::vector<int>&, std::vector<int>&, std::vector<std::vector<double>>&);
    const int max_state_value;
private:
    const int cost_per_action;
    const int reward_per_outflow;
    const double reward_discount;
    int num_states;
    Poisson poisson;
    std::vector<std::pair<int, int>> mean_rates;
    double StateTransitionProb(std::vector<std::pair<int, int>>&);
    int ComputeActionCost(std::vector<int>&);
    double DiscountedFutureReward(std::vector<int>&, std::vector<int>&, int, int, int, int,
                                  std::vector<std::vector<double>>&);
    int BoundsCheckState(int);
};

#endif /* state_action_model_hpp */
