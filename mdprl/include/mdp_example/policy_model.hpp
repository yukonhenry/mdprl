//
//  policy_model.hpp
//  mdprl
//
//  Created by Henry Tominaga on 9/14/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef policy_model_hpp
#define policy_model_hpp

#include <iostream>
#include <map>
#include <vector>
#include "state_action_model.hpp"
class PolicyModel {
public:
    PolicyModel(const int max_value);
    void EvaluatePolicy(StateActionModel&);
    bool ImprovePolicy(StateActionModel&);
    bool PolicyIteration(StateActionModel&);
    std::pair<std::vector<int>, double> FindBestActionForState(StateActionModel&, std::vector<int>&, std::vector<int>&);
private:
    double EvaluatePolicyIterateStates(StateActionModel&);
    std::map<std::vector<int>, std::vector<int>> policy;
    std::vector<std::vector<double>> value_matrix;
    std::pair<std::vector<int>, double> FindBestActionForPartialState(StateActionModel&, std::vector<int>&, std::vector<int>&,
                                                                      std::pair<std::vector<int>, double>, int);
    int max_state_value;
};

#endif /* policy_model_hpp */
