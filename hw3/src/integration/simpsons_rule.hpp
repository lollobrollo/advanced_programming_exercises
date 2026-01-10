#pragma once

#include "integration_strategy.hpp"

namespace SciCpp{

class SimpsonsRule : public IntegrationStrategy {
// Simpson's rule integration strategy
public:
    std::vector<double> getWeights(size_t n_intervals) const override;
    std::vector<double> getNodes(size_t n_intervals) const override;
    double getScalingFactor(size_t n_intervals, double a, double b) const override;
    std::unique_ptr<IntegrationStrategy> clone() const override;
};

} // namespace SciCpp