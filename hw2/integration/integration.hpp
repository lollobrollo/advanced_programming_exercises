#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstddef>

#include "integration_strategy.hpp"

class IntegralEvaluator {
// Class responsible for performing integration tasks
public:
    IntegralEvaluator(std::string function_expression, std::unique_ptr<IntegrationStrategy> strategy);
    double operator()(double a, double b, size_t n_intervals) const;
private:
    std::string function_expression;
    std::unique_ptr<IntegrationStrategy> strategy;
};