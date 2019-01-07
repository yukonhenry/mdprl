//
//  poisson.hpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#ifndef poisson_hpp
#define poisson_hpp
#include <iostream>
#include <vector>
class Poisson {
public:
    Poisson(int max_events): factorial_terms(max_events, 1), factorial_term_already_computed(1) {};
    double ProbabilityForSample(int rate, int n);
private:
    std::vector<unsigned long> factorial_terms;
    unsigned long factorial(int n);
    int factorial_term_already_computed;
};
#endif /* poisson_hpp */
