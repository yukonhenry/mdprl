//
//  monte_carlo_policy_model.cpp
//  mdprl
//
//  Created by Henry Tominaga on 11/11/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <tuple>
#include "monte_carlo_policy_model.hpp"
#include "vehicle_physics.hpp"
const int kMaxInitialValue = 10;
const int kRewardPerStep = -1;
const double kUniDistUpperBound = 2.0;

//https://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
//http://www.pcg-random.org/posts/cpps-random_device.html
//https://gist.github.com/PhDP/5289449
MonteCarloPolicyModel::MonteCarloPolicyModel(const int grid_height, const int grid_width, const int max_action):
uni_dist(0.0, kUniDistUpperBound), rand_gen(std::random_device{}()), max_positive_action(max_action),
total_action_count(2*max_action + 1), action_range(1.0/(2*max_action+1)),
cumulative_weight_matrix(grid_width,
                         std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>>
                         (grid_height,
                          std::vector<std::vector<std::vector<std::vector<double>>>>
                          (grid_width,
                           std::vector<std::vector<std::vector<double>>>
                           (grid_height,
                            std::vector<std::vector<double>>
                            (2*max_action + 1,
                             std::vector<double>(2*max_action + 1)))))) {
                                double default_cost = double(-grid_width * grid_height);
    for (int i = 0; i < grid_width; ++i) {
        std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> state_y_matrix;
        for (int j = 0; j < grid_height; ++j) {
            std::vector<std::vector<std::vector<std::vector<double>>>> velocity_matrix;
            for (int k = 0; k < grid_width; ++k) {
                std::vector<std::vector<std::vector<double>>> velocity_y_matrix;
                for (int l = 0; l < grid_height; ++l) {
                    std::vector<std::vector<double>> action_matrix;
                    for (int m = 0; m < total_action_count; ++m) {
                        std::vector<double> action_y_vector;
                        for (int n = 0; n < total_action_count; ++n) {
                            //double random_pos = double(std::rand() % kMaxInitialValue);
                            //action_y_vector.push_back(-random_pos);
                            action_y_vector.push_back(default_cost);
                        }
                        action_matrix.push_back(action_y_vector);
                    }
                    velocity_y_matrix.push_back(action_matrix);
                }
                velocity_matrix.push_back(velocity_y_matrix);
            }
            state_y_matrix.push_back(velocity_matrix);
        }
        state_action_value_matrix.push_back(state_y_matrix);
    }
    for (int i = 0; i < grid_width; ++i)
        for (int j = 0; j < grid_height; ++j)
            for (int k = 0; k < grid_width; ++k)
                for (int l = 0; l < grid_height; ++l) {
                    double max_value = double(INT_MIN);
                    for (int m = 0; m < total_action_count; ++m)
                        for (int n = 0; n < total_action_count; ++n) {
                            double current_value = state_action_value_matrix.at(i).at(j).at(k).at(l).at(m).at(n);
                            if (current_value > max_value)  {
                                policy[{i, j, k, l}] = {m, n};
                                max_value = current_value;
                            }
                        }
                }
}

std::pair<int, double> MonteCarloPolicyModel::GenerateRandomAction(int default_value) {
    double raw_unidist_sample = uni_dist(rand_gen);
    double b_prob_for_default_value = 1.0 / kUniDistUpperBound;
    if (raw_unidist_sample > 1.0) {
        return std::make_pair(default_value, b_prob_for_default_value);
    } else {
        int absolute_action_value = static_cast<int>(std::floor(raw_unidist_sample / action_range));
        return std::make_pair(absolute_action_value - max_positive_action,
                              b_prob_for_default_value * action_range);
    }
}

std::pair<Accel, BPolicyProb> MonteCarloPolicyModel::ExecuteBPolicy(RaceTrackOperations& operations, RaceTrackVehicle& vehicle) {
    int default_x_action = 0;
    int default_y_action = 0;
    double b_prob;
    if (vehicle.position.x <= operations.racetrack_width / 2) {
        default_x_action = max_positive_action;
    }
    
    if (vehicle.position.y <= operations.racetrack_height / 2) {
        default_y_action = max_positive_action;
        b_prob = 0.5 * action_range;
    }
    auto results_x = GenerateRandomAction(default_x_action);
    auto results_y = GenerateRandomAction(default_y_action);
    int accel_x = results_x.first;
    int accel_y = results_y.first;
    double bprob_x = results_x.second;
    double bprob_y = results_y.second;
    return std::make_pair(Accel(accel_x, accel_y), BPolicyProb(bprob_x, bprob_y));
}

std::pair<Accel, BPolicyProb> MonteCarloPolicyModel::ExecuteValidBPolicyUpdateVehicle(RaceTrackOperations& operations,
                                                             RaceTrackVehicle& vehicle) {
    bool validity_flag = false;
    auto bpolicy_accel_prob = ExecuteBPolicy(operations, vehicle);
    while (!validity_flag) {
        Position test_position = VehicleMoveWithAccel(vehicle.position, vehicle.velocity, bpolicy_accel_prob.first);
        if (test_position.x != vehicle.position.x || test_position.y != vehicle.position.y) {
            validity_flag = true;
            vehicle.UpdateVehicleWithAccelAction(bpolicy_accel_prob.first);
        } else {
            bpolicy_accel_prob = ExecuteBPolicy(operations, vehicle);
        }
    }
    return bpolicy_accel_prob;
}

void MonteCarloPolicyModel::EpisodeUntilGoalOrWall(std::vector<std::tuple<Position, Velocity, Accel, int,
                                                   BPolicyProb>>& state_actions,
                                                   RaceTrackOperations& operations, RaceTrackVehicle& vehicle) {
    while (vehicle.position_status == TrackPositionStatus::ON_TRACK) {
        //Accel bpolicy_accel = ExecuteBPolicy();
        //vehicle.UpdateVehicleWithAccelAction(bpolicy_accel);
        auto accel_prob = ExecuteValidBPolicyUpdateVehicle(operations, vehicle);
        Accel accel = accel_prob.first;
        BPolicyProb prob = accel_prob.second;
        vehicle.position_status = operations.detectTrackPositionStatus(vehicle.position, vehicle.old_position,
                                                                       vehicle.velocity);
        state_actions.push_back(std::make_tuple(vehicle.old_position, vehicle.old_velocity, accel, kRewardPerStep, prob));
        /*
        std::cout << "old position: " << vehicle.old_position.x << " " << vehicle.old_position.y <<
        " velocity:" << vehicle.velocity.x << " " << vehicle.velocity.y <<
        " accel acction:" << bpolicy_accel.x << " " << bpolicy_accel.y <<
        " new position: " << vehicle.position.x << " " << vehicle.position.y << std::endl; */
    }
}

std::vector<std::tuple<Position, Velocity, Accel, int, BPolicyProb>>
MonteCarloPolicyModel::ExecuteEpisode(RaceTrackOperations& operations, RaceTrackVehicle& vehicle) {
    std::vector<std::tuple<Position, Velocity, Accel, int, BPolicyProb>> state_actions;
    while (vehicle.position_status != TrackPositionStatus::GOAL) {
        EpisodeUntilGoalOrWall(state_actions, operations, vehicle);
        if (vehicle.position_status == TrackPositionStatus::GOAL) {
            //std::cout << "state action vec size:" << state_actions.size() << std::endl;
            //std::cout << "goal reached !!" << std::endl;
        } else if (vehicle.position_status == TrackPositionStatus::OFF_TRACK) {
            //std::cout << "out of bounds!!!" << std::endl;
            vehicle.ResetVehicle(operations.RandomizeStartLocation());
        } else {
            throw std::logic_error("Unexpected TrackPositionStatus!");
        }
    }
    return state_actions;

}

void MonteCarloPolicyModel::DebugPrint(Position& p, Velocity& v, Accel& a, std::string msg) {
    std::cout << msg << std::endl;
    std::cout << "Position:" << p.x << " " << p.y << std::endl;
    std::cout << "Velocity:" << v.x << " " << v.y << std::endl;
    std::cout << "Accel:" << a.x << " " << a.y << std::endl;
}

std::pair<int, int> MonteCarloPolicyModel::MapAccelToIndex(Accel& a) {
    return std::make_pair<int, int>(a.x + max_positive_action, a.y + max_positive_action);
}

void MonteCarloPolicyModel::UpdateCumulativeWeightStateActionMatrix(double weight, int g_reward, Position& p, Velocity& v, Accel& a) {
    //DebugPrint(p, v, a, "cumulative update");
    std::pair<int, int> accel_index = MapAccelToIndex(a);
    int updated_cumulative_weight =
    cumulative_weight_matrix.at(p.x).at(p.y).at(v.x).at(v.y).at(accel_index.first).at(accel_index.second) + weight;
    cumulative_weight_matrix.at(p.x).at(p.y).at(v.x).at(v.y).at(accel_index.first).at(accel_index.second) =
        updated_cumulative_weight;
    double current_state_action_value = state_action_value_matrix.at(p.x).at(p.y).at(v.x).at(v.y).at(accel_index.first).at(accel_index.second);
    double new_state_action_value = current_state_action_value +
    (double(weight) / updated_cumulative_weight)*(g_reward - current_state_action_value);
    state_action_value_matrix.at(p.x).at(p.y).at(v.x).at(v.y).at(accel_index.first).at(accel_index.second) =
        new_state_action_value;
}

std::vector<std::pair<int, int>> MonteCarloPolicyModel::FindMaxActionForQState(Position& p, Velocity& v) {
    auto action_values = state_action_value_matrix.at(p.x).at(p.y).at(v.x).at(v.y);
    double max_value = double(INT_MIN);
    std::vector<std::pair<int, int>> max_actions;
    for (int i = 0; i < total_action_count; ++i) {
        for (int j = 0; j < total_action_count; ++j) {
            double iteration_action_value = action_values.at(i).at(j);
            if (iteration_action_value > max_value) {
                max_value = iteration_action_value;
                max_actions.clear();
                max_actions.push_back(std::make_pair(i, j));
            } else if (iteration_action_value == max_value) {
                max_actions.push_back(std::make_pair(i, j));
            }
        }
    }
    //policy[{p.x, p.y, v.x, v.y}] = max_action;
    return max_actions;
}

bool MonteCarloPolicyModel::CompareMaxActionToCurrentAccelSetPolicy(const std::vector<std::pair<int, int>>& max_actions,
                                                           const Position& p, const Velocity& v, const Accel& accel) {
    bool condition = false;
    for (const auto& action: max_actions) {
        Accel action_accel = Accel(action.first - max_positive_action, action.second- max_positive_action);
        if (action_accel.x == accel.x && action_accel.y == accel.y) {
            condition = true;
            policy[{p.x, p.y, v.x, v.y}] = {action.first, action.second};
            break;
        }
    }
    if (!condition) {
        auto first_elem = max_actions.at(0);
        policy[{p.x, p.y, v.x, v.y}] = {first_elem.first, first_elem.second};
    }
    return condition;
}

std::pair<bool, int> MonteCarloPolicyModel::ExecuteProcessEpisode(RaceTrackOperations& operations,
                                                                  RaceTrackVehicle& vehicle) {
    double weight = 1.0;
    int Greward = 0;
    auto state_action_vector = ExecuteEpisode(operations, vehicle);
    bool complete_episode_flag = true;
    for (std::vector<std::tuple<Position, Velocity, Accel, int, BPolicyProb>>::reverse_iterator rit =
         state_action_vector.rbegin();
        rit != state_action_vector.rend(); ++rit) {
        Position p = std::get<0>(*rit);
        Velocity v = std::get<1>(*rit);
        Accel a = std::get<2>(*rit);
        int reward = std::get<3>(*rit);
        BPolicyProb prob = std::get<4>(*rit);
        Greward += reward;
        UpdateCumulativeWeightStateActionMatrix(weight, Greward, p, v, a);
        auto max_actions = FindMaxActionForQState(p, v);
        if (!CompareMaxActionToCurrentAccelSetPolicy(max_actions, p, v, a)) {
            //std::cout << "Episode policy improvement does not match current action, breaking from episode" << std::endl;
            complete_episode_flag = false;
            break;
        }
        weight = weight / (prob.x * prob.y);
        //std::cout << "****updated Weight:" << weight << std::endl;
    }
    //std::cout << "Greward: " << Greward << std::endl;
    return std::make_pair(complete_episode_flag, Greward);
}

void MonteCarloPolicyModel::DumpPolicy(RaceTrackOperations& operations) {
    for (const auto& start_index: operations.starting_row_indices) {
        std::cout << "--start position:" << start_index << std::endl;
        auto action = policy[{start_index, 0, 0, 0}];
        std::cout << "initial_action: " << action.at(0) << " " << action.at(1) << std::endl;
        
    }
    /*
    for (const auto& state_to_action: policy) {
        std::cout << "state:";
        for (const auto& state: state_to_action.first) {
            std::cout << state << " ";
        }
        std::cout << "action: ";
        for (const auto& action: state_to_action.second) {
            std::cout << action << " ";
        }
        std::cout << std::endl;
    } */
}
