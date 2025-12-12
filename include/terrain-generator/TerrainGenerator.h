/**
* @file TerrainGenerator.h
 * @brief Poisson-based terrain reconstruction from gradient fields
 */
#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <Eigen/Core>

/**
 * @class TerrainGenerator
 * @brief Reconstructs heightmap from gradient fields by solving Poisson equation
 *
 * Solves: ∇²z = ∂(dx)/∂x + ∂(dy)/∂y
 * Uses conjugate gradient method with 5-point stencil discretization.
 */
class TerrainGenerator {
public:
    /**
     * @brief Constructor with gradient fields
     * @param dx Partial derivatives in X direction (∂z/∂x)
     * @param dy Partial derivatives in Y direction (∂z/∂y)
     * @pre dx and dy must have same dimensions
     */
    TerrainGenerator(const Eigen::MatrixXd& dx, const Eigen::MatrixXd& dy)
        : m_dx(dx), m_dy(dy) {}

    /**
     * @brief Generate heightmap by solving Poisson equation
     * @return Reconstructed heightmap with same dimensions as input
     * @throws std::invalid_argument if matrices have different dimensions
     * @note Solution is unique up to additive constant (use normalize if needed)
     */
    Eigen::MatrixXd generate();

private:
    Eigen::MatrixXd m_dx;  ///< Gradient in X direction
    Eigen::MatrixXd m_dy;  ///< Gradient in Y direction
};

#endif