//
//  sarsa_policy_model.cpp
//  mdprl
//
//  Created by Henry Tominaga on 1/13/19.
//  Copyright © 2019 Henry Tominaga. All rights reserved.
//
#include <stdexcept>
#include "sarsa_policy_model.hpp"

SarsaPolicyModel::SarsaPolicyModel(const int width, const int height, const int max_action, const Position& p, const Position& g):
    distribution(0, max_action - 1), start(p), goal(g), max_action(max_action) {
        if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height ||
            g.x < 0 || g.x >= width || g.y < 0 || g.y >= height) {
            throw std::out_of_range("Start or Goal beyond range");
        }
        double default_cost = double(-width * height);
        for (int i = 0; i < width; ++i) {
            std::vector<std::vector<double>> state_y_action_matrix;
                for (int j = 0; j < height; ++j) {
                    std::vector<double> action_vector(max_action, default_cost);
                    state_y_action_matrix.push_back(action_vector);
                }
                state_action_q_matrix.push_back(state_y_action_matrix);
        }
        for (int i = 0; i < max_action; ++ i)
            state_action_q_matrix.at(goal.x).at(goal.y).at(i) = 0;
}

int SarsaPolicyModel::FindBestAction(Position& state) {
    double max_value = double(INT_MIN);
    int max_action = -1;
    for (int action = 0; action < max_action; ++action) {
        double value = state_action_q_matrix.at(start.x).at(start.y).at(action);
        if (value > max_value) {
            max_action = action;
            max_value = value;
        }
    }
    return max_action;
}

void SarsaPolicyModel::ExecuteEpisode() {
    Position state = start;
    while (state != goal) {
        
    }
}
