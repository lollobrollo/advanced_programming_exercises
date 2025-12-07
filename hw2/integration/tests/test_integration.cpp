#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cmath>
#include <memory>

#include "integration/integration.hpp"
#include "integration/midpoint_rule.hpp"
#include "integration/trapezoidal_rule.hpp"
#include "integration/simpsons_rule.hpp"
#include "integration/gaussian_quadrature.hpp"

// Test MidpointRule weights
TEST(MidpointRuleTest, WeightsForTwoIntervals) {
    MidpointRule rule;
    auto weights = rule.getWeights(2);
    
    ASSERT_EQ(weights.size(), 2u);
    // Midpoint rule: each interval has equal weight of 1.0
    EXPECT_DOUBLE_EQ(weights[0], 1.0);
    EXPECT_DOUBLE_EQ(weights[1], 1.0);
}

TEST(MidpointRuleTest, WeightsForFourIntervals) {
    MidpointRule rule;
    auto weights = rule.getWeights(4);
    
    ASSERT_EQ(weights.size(), 4u);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_DOUBLE_EQ(weights[i], 1.0);
    }
}

TEST(MidpointRuleTest, NodesForTwoIntervals) {
    MidpointRule rule;
    auto nodes = rule.getNodes(2);
    
    ASSERT_EQ(nodes.size(), 2u);
    // Midpoint rule: nodes at midpoints [0.25, 0.75] for [0,1]
    EXPECT_DOUBLE_EQ(nodes[0], 0.25);
    EXPECT_DOUBLE_EQ(nodes[1], 0.75);
}

TEST(MidpointRuleTest, NodesForFourIntervals) {
    MidpointRule rule;
    auto nodes = rule.getNodes(4);
    
    ASSERT_EQ(nodes.size(), 4u);
    // Midpoint rule: nodes at [0.125, 0.375, 0.625, 0.875]
    EXPECT_DOUBLE_EQ(nodes[0], 0.125);
    EXPECT_DOUBLE_EQ(nodes[1], 0.375);
    EXPECT_DOUBLE_EQ(nodes[2], 0.625);
    EXPECT_DOUBLE_EQ(nodes[3], 0.875);
}

// Test TrapezoidalRule weights
TEST(TrapezoidalRuleTest, WeightsForTwoIntervals) {
    TrapezoidalRule rule;
    auto weights = rule.getWeights(2);
    
    ASSERT_EQ(weights.size(), 3u); // n_intervals + 1 points
    // Trapezoidal rule: endpoints have weight 0.5, interior points have weight 1.0
    EXPECT_DOUBLE_EQ(weights[0], 0.5);
    EXPECT_DOUBLE_EQ(weights[1], 1.0);
    EXPECT_DOUBLE_EQ(weights[2], 0.5);
}

TEST(TrapezoidalRuleTest, WeightsForFourIntervals) {
    TrapezoidalRule rule;
    auto weights = rule.getWeights(4);
    
    ASSERT_EQ(weights.size(), 5u);
    EXPECT_DOUBLE_EQ(weights[0], 0.5);
    EXPECT_DOUBLE_EQ(weights[1], 1.0);
    EXPECT_DOUBLE_EQ(weights[2], 1.0);
    EXPECT_DOUBLE_EQ(weights[3], 1.0);
    EXPECT_DOUBLE_EQ(weights[4], 0.5);
}

TEST(TrapezoidalRuleTest, NodesForTwoIntervals) {
    TrapezoidalRule rule;
    auto nodes = rule.getNodes(2);
    
    ASSERT_EQ(nodes.size(), 3u);
    // Trapezoidal rule: uniformly spaced nodes [0, 0.5, 1]
    EXPECT_DOUBLE_EQ(nodes[0], 0.0);
    EXPECT_DOUBLE_EQ(nodes[1], 0.5);
    EXPECT_DOUBLE_EQ(nodes[2], 1.0);
}

TEST(TrapezoidalRuleTest, NodesForFourIntervals) {
    TrapezoidalRule rule;
    auto nodes = rule.getNodes(4);
    
    ASSERT_EQ(nodes.size(), 5u);
    // Trapezoidal rule: uniformly spaced nodes [0, 0.25, 0.5, 0.75, 1]
    EXPECT_DOUBLE_EQ(nodes[0], 0.0);
    EXPECT_DOUBLE_EQ(nodes[1], 0.25);
    EXPECT_DOUBLE_EQ(nodes[2], 0.5);
    EXPECT_DOUBLE_EQ(nodes[3], 0.75);
    EXPECT_DOUBLE_EQ(nodes[4], 1.0);
}

// Test SimpsonsRule weights
TEST(SimpsonsRuleTest, WeightsForTwoIntervals) {
    SimpsonsRule rule;
    auto weights = rule.getWeights(2);
    
    ASSERT_EQ(weights.size(), 5u);  // 2*2 + 1 = 5
    // Simpson's rule: composite pattern [1, 4, 2, 4, 1]
    EXPECT_DOUBLE_EQ(weights[0], 1.0);
    EXPECT_DOUBLE_EQ(weights[1], 4.0);
    EXPECT_DOUBLE_EQ(weights[2], 2.0);
    EXPECT_DOUBLE_EQ(weights[3], 4.0);
    EXPECT_DOUBLE_EQ(weights[4], 1.0);
}

TEST(SimpsonsRuleTest, WeightsForFourIntervals) {
    SimpsonsRule rule;
    auto weights = rule.getWeights(4);
    
    ASSERT_EQ(weights.size(), 9u);  // 2*4 + 1 = 9
    // Simpson's rule: composite pattern [1, 4, 2, 4, 2, 4, 2, 4, 1]
    EXPECT_DOUBLE_EQ(weights[0], 1.0);
    EXPECT_DOUBLE_EQ(weights[1], 4.0);
    EXPECT_DOUBLE_EQ(weights[2], 2.0);
    EXPECT_DOUBLE_EQ(weights[3], 4.0);
    EXPECT_DOUBLE_EQ(weights[4], 2.0);
    EXPECT_DOUBLE_EQ(weights[5], 4.0);
    EXPECT_DOUBLE_EQ(weights[6], 2.0);
    EXPECT_DOUBLE_EQ(weights[7], 4.0);
    EXPECT_DOUBLE_EQ(weights[8], 1.0);
}

TEST(SimpsonsRuleTest, NodesForTwoIntervals) {
    SimpsonsRule rule;
    auto nodes = rule.getNodes(2);
    
    ASSERT_EQ(nodes.size(), 5u);  // 2*2 + 1 = 5
    // Simpson's rule: uniformly spaced nodes [0, 0.25, 0.5, 0.75, 1]
    EXPECT_DOUBLE_EQ(nodes[0], 0.0);
    EXPECT_DOUBLE_EQ(nodes[1], 0.25);
    EXPECT_DOUBLE_EQ(nodes[2], 0.5);
    EXPECT_DOUBLE_EQ(nodes[3], 0.75);
    EXPECT_DOUBLE_EQ(nodes[4], 1.0);
}

TEST(SimpsonsRuleTest, NodesForFourIntervals) {
    SimpsonsRule rule;
    auto nodes = rule.getNodes(4);
    
    ASSERT_EQ(nodes.size(), 9u);  // 2*4 + 1 = 9
    // Simpson's rule: uniformly spaced nodes with 8 equal intervals
    EXPECT_DOUBLE_EQ(nodes[0], 0.0);
    EXPECT_DOUBLE_EQ(nodes[1], 0.125);
    EXPECT_DOUBLE_EQ(nodes[2], 0.25);
    EXPECT_DOUBLE_EQ(nodes[3], 0.375);
    EXPECT_DOUBLE_EQ(nodes[4], 0.5);
    EXPECT_DOUBLE_EQ(nodes[5], 0.625);
    EXPECT_DOUBLE_EQ(nodes[6], 0.75);
    EXPECT_DOUBLE_EQ(nodes[7], 0.875);
    EXPECT_DOUBLE_EQ(nodes[8], 1.0);
}

TEST(SimpsonsRuleTest, NumberOfNodesForVariousIntervals) {
    SimpsonsRule rule;
    
    // Simpson's rule should have 2*n_intervals + 1 nodes
    EXPECT_EQ(rule.getNodes(1).size(), 3u);   // 2*1 + 1 = 3
    EXPECT_EQ(rule.getNodes(2).size(), 5u);   // 2*2 + 1 = 5
    EXPECT_EQ(rule.getNodes(3).size(), 7u);   // 2*3 + 1 = 7
    EXPECT_EQ(rule.getNodes(4).size(), 9u);   // 2*4 + 1 = 9
    EXPECT_EQ(rule.getNodes(10).size(), 21u); // 2*10 + 1 = 21
}

// Test GaussianQuadrature weights
TEST(GaussianQuadratureTest, WeightsForTwoIntervals) {
    GaussianQuadrature rule;
    auto weights = rule.getWeights(2);
    
    ASSERT_EQ(weights.size(), 2u);
    // For 2-point Gauss-Legendre: both weights should be 1.0
    EXPECT_DOUBLE_EQ(weights[0], 1.0);
    EXPECT_DOUBLE_EQ(weights[1], 1.0);
}

TEST(GaussianQuadratureTest, WeightsForThreeIntervals) {
    GaussianQuadrature rule;
    auto weights = rule.getWeights(3);
    
    ASSERT_EQ(weights.size(), 3u);
    // For 3-point Gauss-Legendre: weights are [5/9, 8/9, 5/9]
    // Using EXPECT_NEAR due to numerical precision from eigenvalue computation
    EXPECT_NEAR(weights[0], 5.0/9.0, 1e-14);
    EXPECT_NEAR(weights[1], 8.0/9.0, 1e-14);
    EXPECT_NEAR(weights[2], 5.0/9.0, 1e-14);
}

TEST(GaussianQuadratureTest, NodesForTwoIntervals) {
    GaussianQuadrature rule;
    auto nodes = rule.getNodes(2);
    
    ASSERT_EQ(nodes.size(), 2u);
    // For 2-point Gauss-Legendre on [-1,1]: nodes at ±1/sqrt(3)
    // Transformed to [0,1]: (node + 1) / 2
    double expected_node_1 = (1.0 - 1.0/std::sqrt(3.0)) / 2.0;
    double expected_node_2 = (1.0 + 1.0/std::sqrt(3.0)) / 2.0;
    
    EXPECT_NEAR(nodes[0], expected_node_1, 1e-14);
    EXPECT_NEAR(nodes[1], expected_node_2, 1e-14);
}

TEST(GaussianQuadratureTest, NodesForThreeIntervals) {
    GaussianQuadrature rule;
    auto nodes = rule.getNodes(3);
    
    ASSERT_EQ(nodes.size(), 3u);
    // For 3-point Gauss-Legendre on [-1,1]: nodes at [-sqrt(3/5), 0, sqrt(3/5)]
    // Transformed to [0,1]: (node + 1) / 2
    double expected_node_1 = (1.0 - std::sqrt(3.0/5.0)) / 2.0;
    double expected_node_2 = 0.5;
    double expected_node_3 = (1.0 + std::sqrt(3.0/5.0)) / 2.0;
    
    EXPECT_NEAR(nodes[0], expected_node_1, 1e-14);
    EXPECT_NEAR(nodes[1], expected_node_2, 1e-14);
    EXPECT_NEAR(nodes[2], expected_node_3, 1e-14);
}

TEST(GaussianQuadratureTest, NodesAreNonUniform) {
    GaussianQuadrature rule;
    auto nodes = rule.getNodes(4);
    
    ASSERT_EQ(nodes.size(), 4u);
    // Verify that nodes are not uniformly spaced
    // For 4 points, spacing should not be constant
    double spacing1 = nodes[1] - nodes[0];
    double spacing2 = nodes[2] - nodes[1];
    double spacing3 = nodes[3] - nodes[2];
    
    // Verify spacings are different (non-uniform)
    EXPECT_FALSE(std::abs(spacing1 - spacing2) < 1e-10);
    EXPECT_FALSE(std::abs(spacing2 - spacing3) < 1e-10);
}

// Test IntegralEvaluator with constant function
TEST(IntegralEvaluatorTest, ConstantFunctionWithMidpoint) {
    // Integrate f(x) = 1 from 0 to 1 should give 1.0
    auto midpoint = std::make_unique<MidpointRule>();
    IntegralEvaluator evaluator("1", std::move(midpoint));
    
    double result = evaluator(0.0, 1.0, 10);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(IntegralEvaluatorTest, ConstantFunctionWithTrapezoidal) {
    // Integrate f(x) = 1 from 0 to 1 should give 1.0
    auto trapezoidal = std::make_unique<TrapezoidalRule>();
    IntegralEvaluator evaluator("1", std::move(trapezoidal));
    
    double result = evaluator(0.0, 1.0, 10);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(IntegralEvaluatorTest, ConstantFunctionWithSimpson) {
    // Integrate f(x) = 1 from 0 to 1 should give 1.0
    auto simpson = std::make_unique<SimpsonsRule>();
    IntegralEvaluator evaluator("1", std::move(simpson));
    
    double result = evaluator(0.0, 1.0, 10);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(IntegralEvaluatorTest, LinearFunctionWithTrapezoidal) {
    // Integrate f(x) = x from 0 to 1 should give 0.5
    auto trapezoidal = std::make_unique<TrapezoidalRule>();
    IntegralEvaluator evaluator("x", std::move(trapezoidal));
    
    double result = evaluator(0.0, 1.0, 100);
    EXPECT_NEAR(result, 0.5, 1e-6);
}

TEST(IntegralEvaluatorTest, QuadraticFunctionWithSimpson) {
    // Integrate f(x) = x^2 from 0 to 1 should give 1/3
    auto simpson = std::make_unique<SimpsonsRule>();
    IntegralEvaluator evaluator("x*x", std::move(simpson));
    
    double result = evaluator(0.0, 1.0, 100);
    EXPECT_NEAR(result, 1.0/3.0, 1e-6);
}

TEST(IntegralEvaluatorTest, ExponentialFunctionWithSimpson) {
    // Integrate f(x) = exp(x) from 0 to 1 should give e - 1
    auto simpson = std::make_unique<SimpsonsRule>();
    IntegralEvaluator evaluator("exp(x)", std::move(simpson));
    
    double result = evaluator(0.0, 1.0, 100);
    EXPECT_NEAR(result, std::exp(1.0) - 1.0, 1e-5);
}

TEST(IntegralEvaluatorTest, SineFunction) {
    // Integrate f(x) = sin(x) from 0 to pi should give 2
    auto simpson = std::make_unique<SimpsonsRule>();
    IntegralEvaluator evaluator("sin(x)", std::move(simpson));
    
    double result = evaluator(0.0, M_PI, 100);
    EXPECT_NEAR(result, 2.0, 1e-5);
}

TEST(IntegralEvaluatorTest, CosineFunction) {
    // Integrate f(x) = cos(x) from 0 to pi/2 should give 1
    auto simpson = std::make_unique<SimpsonsRule>();
    IntegralEvaluator evaluator("cos(x)", std::move(simpson));
    
    double result = evaluator(0.0, M_PI/2.0, 100);
    EXPECT_NEAR(result, 1.0, 1e-5);
}

TEST(IntegralEvaluatorTest, NegativeInterval) {
    // Integrate f(x) = -1 from 0 to 2 should give -2
    auto trapezoidal = std::make_unique<TrapezoidalRule>();
    IntegralEvaluator evaluator("-1", std::move(trapezoidal));
    
    double result = evaluator(0.0, 2.0, 10);
    EXPECT_NEAR(result, -2.0, 1e-6);
}

TEST(IntegralEvaluatorTest, PolynomialExpression) {
    // Integrate f(x) = 2*x + 1 from 0 to 2 should give 6
    // (x^2 + x evaluated from 0 to 2 = 4 + 2 = 6)
    auto simpson = std::make_unique<SimpsonsRule>();
    IntegralEvaluator evaluator("2*x+1", std::move(simpson));
    
    double result = evaluator(0.0, 2.0, 100);
    EXPECT_NEAR(result, 6.0, 1e-5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}