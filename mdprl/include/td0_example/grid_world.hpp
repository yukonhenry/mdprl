//
//  grid_world.hpp
//  mdprl
//
//  Created by Henry Tominaga on 1/13/19.
//  Copyright © 2019 Henry Tominaga. All rights reserved.
//

#ifndef grid_world_hpp
#define grid_world_hpp

#include <iostream>
#include <vector>

struct Position {
    int x;
    int y;
    Position(int x, int y): x(x), y(y) {};
    Position(): x(0), y(0) {};
    // ref https://stackoverflow.com/questions/7204283/struct-as-a-key-in-a-stdmap
    bool const operator==(const Position &p) const {
        return x == p.x && y == p.y;
    }
    
    bool const operator!=(const Position &p) const {
        return x != p.x || y != p.y;
    }
    
    bool const operator<(const Position &p) const {
        return x < p.x || (x == p.x && y < p.y);
}};


class GridWorld {
public:
    GridWorld(std::vector<std::pair<int, int>>& configs);
    std::vector<std::vector<int>> grid;
    std::vector<int> winds;
private:
    Position start, goal;
};
#endif /* grid_world_hpp */
