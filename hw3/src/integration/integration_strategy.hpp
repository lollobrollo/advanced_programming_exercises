#pragma once

#include <vector>
#include <memory>
#include <cstddef>

namespace SciCpp {

class IntegrationStrategy {
// Abstract base class for integration strategies
public:
    virtual ~IntegrationStrategy() = default;
    virtual std::vector<double> getWeights(size_t n_intervals) const = 0;
    // Get the nodes on the interval [0,1] which will be scaled to [a,b]
    virtual std::vector<double> getNodes(size_t n_intervals) const = 0;
    virtual double getScalingFactor(size_t n_intervals, double a, double b) const = 0;
    // Clone method to create a unique_ptr copy of the derived class
    virtual std::unique_ptr<IntegrationStrategy> clone() const = 0;
};

} // namespace SciCpp
