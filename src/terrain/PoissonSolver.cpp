#include "PoissonSolver.h"

PoissonSolver::PoissonSolver(const GradientField& grad) : gradient(grad) {}

Eigen::MatrixXd PoissonSolver::solve() {
    int width = gradient.getWidth();
    int height = gradient.getHeight();
    Eigen::MatrixXd heightMap = Eigen::MatrixXd::Zero(height, width);

    // Aqui você implementaria a solução do Poisson baseado no campo de gradientes
    // Para teste, podemos apenas acumular os gradientes como exemplo simples:
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Eigen::Vector2d g = gradient.getGradient(x, y);
            heightMap(y, x) = g[0] + g[1]; // simplificado
        }
    }

    return heightMap;
}
