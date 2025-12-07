#include "integration.hpp"
#include <stdexcept>
#include <muparserx/mpParser.h>

// Implementation of IntegralEvaluator
IntegralEvaluator::IntegralEvaluator(std::string function_expression, 
                                     std::unique_ptr<IntegrationStrategy> strategy)
    : function_expression(function_expression), strategy(std::move(strategy)) {}

double IntegralEvaluator::operator()(double a, double b, size_t n_intervals) const {
    if (n_intervals == 0) {
        throw std::invalid_argument("n_intervals must be greater than 0");
    }
    
    // Set up muparserx for expression evaluation
    mup::ParserX parser;
    mup::Value x_var(0.0);
    
    try {
        parser.DefineVar("x", mup::Variable(&x_var));
        parser.SetExpr(function_expression);
    } catch (const mup::ParserError& e) {
        throw std::runtime_error("Parser error: " + std::string(e.GetMsg()));
    }
    
    auto weights = strategy->getWeights(n_intervals);
    auto nodes = strategy->getNodes(n_intervals);
    double scaling_factor = strategy->getScalingFactor(n_intervals, a, b);
    
    double result = 0.0;
    
    // Evaluate the function at each node and sum with weights
    for (size_t i = 0; i < nodes.size(); ++i) {
        double x = a + nodes[i] * (b - a);
        
        try {
            // Update the value of x and evaluate
            x_var = x;
            mup::Value eval_result = parser.Eval();
            result += weights[i] * eval_result.GetFloat();
        } catch (const mup::ParserError& e) {
            throw std::runtime_error("Evaluation error: " + std::string(e.GetMsg()));
        }
    }
    
    result *= scaling_factor;
    
    return result;
}
