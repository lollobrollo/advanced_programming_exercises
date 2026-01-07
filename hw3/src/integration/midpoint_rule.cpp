#include "midpoint_rule.hpp"
#include <vector>

namespace SciCpp {

std::vector<double> MidpointRule::getWeights(size_t n_intervals) const {
    return std::vector<double>(n_intervals, 1.0);
}

std::vector<double> MidpointRule::getNodes(size_t n_intervals) const {
    std::vector<double> nodes(n_intervals);
    for (size_t i = 0; i < n_intervals; ++i) {
        nodes[i] = (i + 0.5) / n_intervals;
    }
    return nodes;
}

double MidpointRule::getScalingFactor(size_t n_intervals, double a, double b) const {
    return (b - a) / n_intervals;
}

} // namespace SciCpp