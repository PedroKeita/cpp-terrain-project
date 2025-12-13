/**
 * @file TerrainGenerator.cpp
 * @brief Implementation of TerrainGenerator class for reconstructing
 *        surfaces from gradient fields using the Poisson equation.
 *
 * The algorithm integrates a field of partial derivatives m_dx and m_dy
 * reconstructing the original surface Z(x, y) by solving:
 *
 *      ∆Z = div(gradient field)
 *
 * where the right-hand side is calculated through finite differences of the gradient.
 */

#include "../../include/terrain-generator/TerrainGenerator.h"
#include <Eigen/IterativeLinearSolvers>

/**
 * @brief Generates a height matrix by solving the Poisson equation for the gradient field.
 *
 * The function reconstructs the height map from the derivative fields m_dx and m_dy.
 * The process works in three stages:
 *
 *  1. Assembly of vector b containing the divergence of the gradient field.
 *  2. Construction of the discrete Laplacian matrix using finite differences.
 *  3. Solving the system L * Z = b using the Conjugate Gradient method.
 *
 * @return Eigen::MatrixXd Matrix (h × w) with reconstructed heights.
 *
 * @note This method solves a large sparse system, therefore it is efficient
 *       for large terrains.
 */
Eigen::MatrixXd TerrainGenerator::generate() {
    int h = m_dx.rows(); ///< Number of rows in the map
    int w = m_dx.cols(); ///< Number of columns in the map
    int n = h * w; ///< Total number of variables in the system

    // -------------------------------------------------------------------------
    //  Assemble vector 'b' containing the divergence of the gradient field
    // -------------------------------------------------------------------------

    Eigen::VectorXd b(n);
    b.setZero();

    /**
     * Divergence is calculated through finite differences:
     *
     * div = d/dx(dx) + d/dy(dy)
     *
     * with special treatment for edges.
     */
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int i = y * w + x;
            double divX = 0, divY = 0;

            // ----- d/dx -----
            if (x > 0 && x < w - 1)
                divX = (m_dx(y, x + 1) - m_dx(y, x - 1)) / 2.0;
            else if (x == 0)
                divX = m_dx(y, x + 1) - m_dx(y, x);
            else
                divX = m_dx(y, x) - m_dx(y, x - 1);

            // ----- d/dy -----
            if (y > 0 && y < h - 1)
                divY = (m_dy(y + 1, x) - m_dy(y - 1, x)) / 2.0;
            else if (y == 0)
                divY = m_dy(y + 1, x) - m_dy(y, x);
            else
                divY = m_dy(y, x) - m_dy(y - 1, x);

            b(i) = divX + divY;
        }
    }

    // -------------------------------------------------------------------------
    // Construction of the sparse Laplacian matrix (5-diagonal)
    // -------------------------------------------------------------------------

    typedef Eigen::Triplet<double> T;
    std::vector<T> triplets;
    triplets.reserve(5 * n); ///< Each cell contributes up to 5 values

    /**
     * For each cell, we connect:
     *
     *  - Left
     *  - Right
     *  - Up
     *  - Down
     *
     * And at the own index we place the negative value of the neighbor count:
     *
     *      L(i,i) = -deg(i)
     */
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int i = y * w + x;
            int neighbors = 0;

            if (x > 0) { triplets.push_back(T(i, i - 1, 1.0)); neighbors++; }
            if (x < w - 1) { triplets.push_back(T(i, i + 1, 1.0)); neighbors++; }
            if (y > 0) { triplets.push_back(T(i, i - w, 1.0)); neighbors++; }
            if (y < h - 1) { triplets.push_back(T(i, i + w, 1.0)); neighbors++; }

            triplets.push_back(T(i, i, -static_cast<double>(neighbors)));
        }
    }

    Eigen::SparseMatrix<double> L(n, n);
    L.setFromTriplets(triplets.begin(), triplets.end());

    // -------------------------------------------------------------------------
    // Solve the linear system: Lz = b
    // -------------------------------------------------------------------------
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
    solver.compute(L);
    Eigen::VectorXd z = solver.solve(b);

    // -------------------------------------------------------------------------
    // Convert the solution vector to h × w matrix
    // -------------------------------------------------------------------------
    Eigen::MatrixXd result(h, w);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            result(y, x) = z(y * w + x);

    return result;
}