//
//  state_transition_model.cpp
//  mdprl
//
//  Created by Henry Tominaga on 9/8/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include <stack>
#include "config.hpp"
#include "state_action_model.hpp"

StateActionModel::StateActionModel(int max_value, int action_cost, int outflow_reward, double discount):
    max_state_value(max_value), poisson(Poisson(max_value + 1)), cost_per_action(action_cost),
    reward_per_outflow(outflow_reward), reward_discount(discount) {
    mean_rates = ReadDepotConfig("./depot_config.txt");
    num_states = (int) mean_rates.size();
};

double StateActionModel::ComputeStateValue(std::vector<int>& states, std::vector<int>& actions, std::vector<std::vector<double>>& mdp_values) {
    int action_cost = ComputeActionCost(actions);
    int state_0 = states.at(0);
    int state_1 = states.at(1);
    //std::cout << "states:" << states.at(0) << "," << states.at(1) << " actions:" << actions.at(0) << "," << actions.at(1) << std::endl;
    std::vector<std::pair<int, int>> state_inflow_outflows;
    double total_weighted_rewards = 0.0;
    for (int outflow_rental_0 = 0; outflow_rental_0 <= state_0; ++outflow_rental_0) {
        for (int inflow_return_0 = 0; inflow_return_0 <= max_state_value - (state_0 - outflow_rental_0);
             ++inflow_return_0) {
            state_inflow_outflows.push_back(std::make_pair(inflow_return_0, outflow_rental_0));
            for (int outflow_rental_1 = 0; outflow_rental_1 <= state_1; ++outflow_rental_1) {
                for (int inflow_return_1 = 0; inflow_return_1 <= max_state_value - (state_1 - outflow_rental_1);
                     ++inflow_return_1) {
                    state_inflow_outflows.push_back(std::make_pair(inflow_return_1, outflow_rental_1));
                    double prob = StateTransitionProb(state_inflow_outflows);
                    double future_reward = DiscountedFutureReward(states, actions,
                                                                  outflow_rental_0, outflow_rental_1,
                                                                  inflow_return_0, inflow_return_1, mdp_values);
                    int current_reward = reward_per_outflow * (outflow_rental_0 + outflow_rental_1) - action_cost;
                    /*
                    std::cout << "states:" << states.at(0) << "," << states.at(1) <<
                    " outflow_0:" << outflow_rental_0 << " inflow_0:" << inflow_return_0 << " outflow_1:" <<
                    outflow_rental_1 << " inflow_1:" << inflow_return_1 << " prob:" << prob << " reward:" << current_reward <<
                    " future reward:" << future_reward << std::endl; */
                    total_weighted_rewards += prob * (current_reward + future_reward);
                    state_inflow_outflows.pop_back();
                }
            }
            state_inflow_outflows.pop_back();
        }
    }
     return total_weighted_rewards;
}

double StateActionModel::DiscountedFutureReward(std::vector<int>& states, std::vector<int>& actions, int outflow_rental_0,
                                          int outflow_rental_1, int inflow_return_0, int inflow_return_1,
                                          std::vector<std::vector<double>>& mdp_values) {
    int action_0 = actions.at(0);
    int action_1 = actions.at(1);
    int state_0_prime = states.at(0) + inflow_return_0 - outflow_rental_0 + action_1 - action_0;
    state_0_prime = BoundsCheckState(state_0_prime);
    int state_1_prime = states.at(1) + inflow_return_1 - outflow_rental_1 + action_0 - action_1;
    state_1_prime = BoundsCheckState(state_1_prime);
    return reward_discount * mdp_values.at(state_0_prime).at(state_1_prime);
}

int StateActionModel::BoundsCheckState(int state) {
    int lower_checked = state > 0 ? state : 0;
    int upper_checked = lower_checked <= max_state_value ? lower_checked : max_state_value;
    return upper_checked;
}
int StateActionModel::ComputeActionCost(std::vector<int> &actions) {
    int action_sum = 0;
    for (const auto& a: actions) {
        action_sum += a;
    }
    return cost_per_action * action_sum;
}

double StateActionModel::StateTransitionProb(std::vector<std::pair<int, int>>& exogenous_state_inflow_outflow) {
    double prob = 1.0;
    for (int index = 0; index < exogenous_state_inflow_outflow.size(); ++index) {
        auto depot_inflow_outflow = exogenous_state_inflow_outflow.at(index);
        auto depot_rates = mean_rates.at(index);
        double incoming_prob = poisson.ProbabilityForSample(depot_rates.first, depot_inflow_outflow.first);
        double outgoing_prob = poisson.ProbabilityForSample(depot_rates.second, depot_inflow_outflow.second);
        prob *= incoming_prob * outgoing_prob;
    }
    return prob;
}
