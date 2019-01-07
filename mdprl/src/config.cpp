//
//  config.cpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "config.hpp"
#include <tclap/CmdLine.h>
#include <fstream>
#include <cstdio>

Config getConfig(int argc, const char * argv[]) {
    Config c;
    try {
        TCLAP::CmdLine cmd("MDP Implementation", ' ', "0.1");
        
        TCLAP::ValueArg<double> sample_step_arg("s", "simulation_step_size", "Sampling timestep", false, 0.1,"double");
        cmd.add(sample_step_arg);
        
        TCLAP::ValueArg<double> final_time_arg("t", "simulation_final_time", "Execution time", false, 10.0,"double");
        cmd.add(final_time_arg);
        
        cmd.parse( argc, argv );
        c = {
            sample_step_arg.getValue(),
            final_time_arg.getValue(),
        };
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return c;
}

std::vector<std::pair<int, int>> ReadDepotConfig(const char* filename) {
    std::ifstream input_file(filename);
    if (input_file) {
        std::string line;
        std::vector< std::pair<int, int> > parameters;
        while ( getline(input_file, line ) ) {
            std::istringstream is( line );
            std::istream_iterator<double> iit (is);
            std::istream_iterator<double> eos;
            if (iit != eos) {
                int first, second;
                first = *iit;
                ++iit;
                if (iit != eos) {
                    second = *iit;
                    parameters.push_back(std::make_pair(first, second));
                }
            }
        }
        input_file.close();
        return parameters;
    } else {
        std::perror(filename);
        std::cout << "Unable to open:" << filename << std::endl;
        exit(1); // terminate with error
    }
}

std::vector<std::vector<int>> ReadRaceTrackConfig(const char* filename) {
    std::ifstream input_file(filename);
    if (input_file) {
        std::string line;
        std::vector< std::vector<int> > race_track_grid;
        while ( getline(input_file, line ) ) {
            std::istringstream is( line );
            std::istream_iterator<double> iit (is);
            std::istream_iterator<double> eos;
            std::vector<int> row;
            while (iit != eos) {
                row.push_back(*iit);
                ++iit;
            }
            race_track_grid.push_back(row);
        }
        // reverse row numbering so that bottom of track is index 0
        // race track grid expressed by geographic coordinates where 0, 0 is bottom left corner.
        // different from matrix coordinates
        std::reverse(race_track_grid.begin(), race_track_grid.end());
        input_file.close();
        return race_track_grid;
    } else {
        std::perror(filename);
        std::cout << "Unable to open:" << filename << std::endl;
        exit(1); // terminate with error
    }
}
