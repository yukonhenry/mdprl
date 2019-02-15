//
//  monte_carlo_policy_model.hpp
//  mdprl
//
//  Created by Henry Tominaga on 11/11/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef monte_carlo_policy_model_hpp
#define monte_carlo_policy_model_hpp

#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include "racetrack_operations.hpp"
#include "racetrack_vehicle.hpp"
struct BPolicyProb {
    double x;
    double y;
    BPolicyProb(double x, double y): x(x), y(y) {};
    BPolicyProb(): x(0.0), y(0.0) {};
};

class MonteCarloPolicyModel {
public:
    MonteCarloPolicyModel(const int grid_height, const int grid_width, const int max_positive_action);
    std::vector<std::tuple<Position, Velocity, Accel, int, BPolicyProb>>
    ExecuteEpisode(RaceTrackOperations& operations, RaceTrackVehicle& vehicle);
    std::pair<bool, int> ExecuteProcessEpisode(RaceTrackOperations& operations, RaceTrackVehicle& vehicle);
    std::pair<Accel, BPolicyProb> ExecuteBPolicy(RaceTrackOperations&, RaceTrackVehicle&);
    void DumpPolicy(RaceTrackOperations&);
private:
    std::vector<
    std::vector<
    std::vector<
    std::vector<
    std::vector<
    std::vector<double>>>>>> state_action_value_matrix;
    std::map<std::vector<int>, std::vector<int>> policy;
    std::vector<
    std::vector<
    std::vector<
    std::vector<
    std::vector<
    std::vector<double>>>>>> cumulative_weight_matrix;
    std::uniform_real_distribution<> uni_dist;
    std::mt19937 rand_gen;
    const int max_positive_action;
    const double action_range;
    const int total_action_count;
    std::pair<int, double> GenerateRandomAction(int);
    std::pair<Accel, BPolicyProb> ExecuteValidBPolicyUpdateVehicle(RaceTrackOperations&, RaceTrackVehicle&);
    void EpisodeUntilGoalOrWall(std::vector<std::tuple<Position, Velocity, Accel, int, BPolicyProb>>& state_actions,
                               RaceTrackOperations& operations,
                               RaceTrackVehicle& vehicle);
    void UpdateCumulativeWeightStateActionMatrix(double weight, int g_reward, Position&, Velocity&, Accel&);
    std::vector<std::pair<int, int>> FindMaxActionForQState(Position&, Velocity&);
    bool CompareMaxActionToCurrentAccelSetPolicy(const std::vector<std::pair<int, int>>&, const Position& p, const Velocity& v, const Accel&);
    
    void DebugPrint(Position&, Velocity&, Accel&, std::string);
    std::pair<int, int> MapAccelToIndex(Accel&);
};

#endif /* monte_carlo_policy_model_hpp */
