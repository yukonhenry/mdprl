//
//  test_sarsa_policy_model.cpp
//  TestMDPRL
//
//  Created by Henry Tominaga on 1/14/19.
//  Copyright © 2019 Henry Tominaga. All rights reserved.
//

#include "catch.hpp"
#include "sarsa_policy_model.hpp"

TEST_CASE("Create Q Matrrix") {
    SarsaPolicyModel model = SarsaPolicyModel(3, 5, 4, Position(1, 0), Position(2, 4));
    double expected_value = -15.0;
    REQUIRE(model.state_action_q_matrix.size() == 3);
    REQUIRE(model.state_action_q_matrix.at(0).size() == 5);
    REQUIRE(model.state_action_q_matrix.at(0).at(0).at(0) == expected_value);
    REQUIRE(model.state_action_q_matrix.at(2).at(3).at(0) == expected_value);
    REQUIRE(model.state_action_q_matrix.at(2).at(3).at(3) == expected_value);

    REQUIRE(model.state_action_q_matrix.at(2).at(4).at(0) == 0);
    REQUIRE(model.state_action_q_matrix.at(2).at(4).at(1) == 0);
    REQUIRE(model.state_action_q_matrix.at(2).at(4).at(3) == 0);
}

