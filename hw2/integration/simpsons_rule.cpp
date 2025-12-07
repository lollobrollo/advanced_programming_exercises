#include "simpsons_rule.hpp"
#include <vector>

std::vector<double> SimpsonsRule::getWeights(size_t n_intervals) const {
    // Composite Simpson's rule requires 2*n_intervals + 1 points
    size_t n_points = 2 * n_intervals + 1;
    std::vector<double> weights(n_points);
    
    for (size_t i = 0; i < n_points; ++i) {
        if (i == 0 || i == n_points - 1) {
            weights[i] = 1.0;
        } else if (i % 2 == 1) {
            weights[i] = 4.0;
        } else {
            weights[i] = 2.0;
        }
    }
    
    return weights;
}

std::vector<double> SimpsonsRule::getNodes(size_t n_intervals) const {
    size_t n_points = 2 * n_intervals + 1;
    std::vector<double> nodes(n_points);
    for (size_t i = 0; i < n_points; ++i) {
        nodes[i] = static_cast<double>(i) / (n_points - 1);
    }
    return nodes;
}

double SimpsonsRule::getScalingFactor(size_t n_intervals, double a, double b) const {
    return (b - a) / (6.0 * n_intervals);
}
