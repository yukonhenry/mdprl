//
//  policy_model.cpp
//  mdprl
//
//  Created by Henry Tominaga on 9/14/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "policy_model.hpp"
#include <algorithm>
#include <cmath>
PolicyModel::PolicyModel(const int max_value): max_state_value(max_value),
    value_matrix(max_value+1, std::vector<double>(max_value+1)) {
    //default policy
        policy[{0, 0}] = {0, 0};
    for (int value2 = 1; value2 <= max_value; ++value2) {
        policy[{0, value2}] = {0, 1};
    }
    for (int value1 = 1; value1 <= max_value; ++value1) {
        for (int value2 = 0; value2 <= max_value; ++value2) {
            policy[{value1, value2}] = {1, 0};
        }
    }
}

double PolicyModel::EvaluatePolicyIterateStates(StateActionModel& state_model) {
    double residual = 0.0;
    for (int state_1 = 0; state_1 <= max_state_value; ++state_1) {
        for (int state_2 = 0; state_2 <= max_state_value; ++state_2) {
            std::vector<int> current_state = {state_1, state_2};
            std::vector<int> current_action = policy.at(current_state);
            double previous_state_value = value_matrix.at(state_1).at(state_2);
            double value = state_model.ComputeStateValue(current_state, current_action, value_matrix);
            residual = std::max(residual, std::abs(value - previous_state_value));
            value_matrix.at(state_1).at(state_2) = value;
            //std::cout << "state:" << state_1 << "," << state_2 <<
            //" value:" << value << " previous:" << previous_state_value << " residual:" << residual <<
            //std::endl << "========" << std::endl;

        }
    }
    return residual;
}

bool PolicyModel::PolicyIteration(StateActionModel& state_model) {
    bool policy_stable = true;
    int iteration_count = 0;
    do {
        EvaluatePolicy(state_model);
        policy_stable = ImprovePolicy(state_model);
        std::cout << "POLICY Iteration  count" << iteration_count << std::endl;
        std::cout << "**********************" << std::endl;
        ++iteration_count;
    } while (!policy_stable);
    return policy_stable;
}
void PolicyModel::EvaluatePolicy(StateActionModel& state_model) {
    double residual = 1e6;
    unsigned iteration_count = 0;
    while (residual > 1e-2){
        residual = EvaluatePolicyIterateStates(state_model);
        std::cout << "***Evaluation Iteration:" << ++iteration_count << " residual:" << residual << std::endl;
    }
}

bool PolicyModel::ImprovePolicy(StateActionModel& state_model) {
    bool policy_stable = true;
    for (int state_1 = 0; state_1 <= max_state_value; ++state_1) {
        for (int state_2 = 0; state_2 <= max_state_value; ++state_2) {
            std::vector<int> current_state = {state_1, state_2};
            std::vector<int> current_action = policy.at(current_state);
            auto best_action_value = FindBestActionForState(state_model, current_state, current_action);
            if (best_action_value.first == current_action) {
                std::cout << "states:" << state_1 << "," <<state_2 << " Policy STABLE with action:" <<
                current_action.at(0) << "," << current_action.at(1) << std::endl;
            }
            else {
                policy_stable = false;
                policy.at(current_state) = best_action_value.first;
                value_matrix.at(state_1).at(state_2) = best_action_value.second;
                std::cout << "states:" << state_1 << "," <<state_2 << " Policy CHANGED to action:" <<
                best_action_value.first.at(0) << "," << best_action_value.first.at(1) <<
                " with value:" << best_action_value.second << std::endl;
            }
        }
    }
    return policy_stable;
}

std::pair<std::vector<int>, double> PolicyModel::FindBestActionForState(StateActionModel& state_model,
                                                                        std::vector<int>& current_state,
                                                                        std::vector<int>& reference_action) {
    double current_max_value = 0.0;
    auto best_action_value =
        FindBestActionForPartialState(state_model, current_state, reference_action,
                                      std::make_pair(reference_action, current_max_value), 0);
    best_action_value = FindBestActionForPartialState(state_model, current_state, reference_action,
                                                      best_action_value, 1);
    return best_action_value;
}

std::pair<std::vector<int>, double> PolicyModel::
    FindBestActionForPartialState(StateActionModel& state_model, std::vector<int>& current_state,
                                  std::vector<int>& reference_action,
                                  std::pair<std::vector<int>, double> current_best_action_value,
                                  int index) {
        std::vector<int> best_action = current_best_action_value.first;
        double max_value = current_best_action_value.second;
        for (int action = 0; action <= current_state.at(index); ++action) {
            //if (action == reference_action.at(index))
            //    continue;
            std::vector<int> proposed_action;
            if (index == 0)
                proposed_action = {action, 0};
            else if (index == 1)
                proposed_action = {0, action};
            double value = state_model.ComputeStateValue(current_state, proposed_action, value_matrix);
            if (value > max_value) {
                max_value = value;
                best_action = proposed_action;
            }
        }
        return std::make_pair(best_action, max_value);
    }



