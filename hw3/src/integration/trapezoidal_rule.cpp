#include "trapezoidal_rule.hpp"
#include <vector>

namespace SciCpp{

std::vector<double> TrapezoidalRule::getWeights(size_t n_intervals) const {
    std::vector<double> weights(n_intervals + 1);
    weights[0] = 0.5;
    weights[n_intervals] = 0.5;
    
    for (size_t i = 1; i < n_intervals; ++i) {
        weights[i] = 1.0;
    }
    
    return weights;
}

std::vector<double> TrapezoidalRule::getNodes(size_t n_intervals) const {
    std::vector<double> nodes(n_intervals + 1);
    for (size_t i = 0; i <= n_intervals; ++i) {
        nodes[i] = static_cast<double>(i) / n_intervals;
    }
    return nodes;
}

double TrapezoidalRule::getScalingFactor(size_t n_intervals, double a, double b) const {
    return (b - a) / n_intervals;
}

} // namespace SciCpp