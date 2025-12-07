#include "gaussian_quadrature.hpp"
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

// Compute Gauss-Legendre nodes and weights using Golub-Welsch algorithm
// Source: https://www.ams.org/journals/mcom/1969-23-106/S0025-5718-69-99647-1/S0025-5718-69-99647-1.pdf
void computeGaussLegendreNodesAndWeights(size_t n, std::vector<double>& nodes, std::vector<double>& weights) {
    // Create tridiagonal matrix for Legendre polynomials (Jacobi matrix)
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(n, n);
    
    // Diagonal elements (all zeros for Legendre)
    for (size_t i = 0; i < n; ++i) {
        J(i, i) = 0.0;
    }
    
    // Off-diagonal elements
    for (size_t i = 0; i < n - 1; ++i) {
        double i_plus_1 = static_cast<double>(i + 1);
        double offdiag_val = i_plus_1 / std::sqrt(4.0 * i_plus_1 * i_plus_1 - 1.0);
        J(i, i + 1) = offdiag_val;
        J(i + 1, i) = offdiag_val;
    }
    
    // Compute eigenvalues and eigenvectors
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(J);
    Eigen::VectorXd eigenvalues = solver.eigenvalues();
    Eigen::MatrixXd eigenvectors = solver.eigenvectors();
    
    // Extract nodes from eigenvalues
    nodes.resize(n);
    for (size_t i = 0; i < n; ++i) {
        nodes[i] = eigenvalues(i);
    }
    
    // Compute weights from first row of eigenvectors
    weights.resize(n);
    double factor = 2.0; // integral of 1 from -1 to 1
    for (size_t i = 0; i < n; ++i) {
        double first_component = eigenvectors(0, i);
        weights[i] = factor * first_component * first_component;
    }
}

std::vector<double> GaussianQuadrature::getWeights(size_t n_intervals) const {
    std::vector<double> nodes, weights;
    computeGaussLegendreNodesAndWeights(n_intervals, nodes, weights);
    return weights;
}

std::vector<double> GaussianQuadrature::getNodes(size_t n_intervals) const {
    std::vector<double> nodes, weights;
    computeGaussLegendreNodesAndWeights(n_intervals, nodes, weights);
    
    // Transform nodes from [-1,1] to [0,1]
    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i] = (nodes[i] + 1.0) / 2.0;
    }
    
    return nodes;
}

double GaussianQuadrature::getScalingFactor(size_t n_intervals, double a, double b) const {
    (void)n_intervals; // Unused parameter (hide warning)
    return (b - a) / 2.0;
}
