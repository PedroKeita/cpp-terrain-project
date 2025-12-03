#include "TerrainGenerator.h"
#include <Eigen/IterativeLinearSolvers>

Eigen::MatrixXd TerrainGenerator::generate() {
    int h = m_dx.rows();
    int w = m_dx.cols();
    int n = h * w;

    // Build Poisson equation: Laplacian(Z) = div(gradient)
    // Using simple finite differences

    Eigen::VectorXd b(n);
    b.setZero();

    // Compute divergence of gradient field
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int i = y * w + x;
            double divX = 0, divY = 0;

            if (x > 0 && x < w - 1)
                divX = (m_dx(y, x + 1) - m_dx(y, x - 1)) / 2.0;
            else if (x == 0)
                divX = m_dx(y, x + 1) - m_dx(y, x);
            else
                divX = m_dx(y, x) - m_dx(y, x - 1);

            if (y > 0 && y < h - 1)
                divY = (m_dy(y + 1, x) - m_dy(y - 1, x)) / 2.0;
            else if (y == 0)
                divY = m_dy(y + 1, x) - m_dy(y, x);
            else
                divY = m_dy(y, x) - m_dy(y - 1, x);

            b(i) = divX + divY;
        }
    }

    // Build Laplacian matrix
    typedef Eigen::Triplet<double> T;
    std::vector<T> triplets;
    triplets.reserve(5 * n);

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

    // Solve with conjugate gradient
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
    solver.compute(L);
    Eigen::VectorXd z = solver.solve(b);

    // Convert to matrix
    Eigen::MatrixXd result(h, w);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            result(y, x) = z(y * w + x);

    return result;
}
