//
//  poisson.cpp
//  mdprl
//
//  Created by Henry Tominaga on 9/3/18.
//  Copyright © 2018 Henry Tominaga. All rights reserved.
//

#include "poisson.hpp"
#include <cmath>
double Poisson::ProbabilityForSample(int mean, int sample) {
    return std::pow(mean, sample) * std::exp(-mean) / factorial(sample);
}

unsigned long Poisson::factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    if (n > factorial_term_already_computed){
        for (int term = factorial_term_already_computed + 1; term < n + 1; ++term) {
            factorial_terms.at(term - 1) = term * factorial_terms.at(term - 2);
        }
        factorial_term_already_computed = n;
    }
    return factorial_terms.at(n - 1);
}
