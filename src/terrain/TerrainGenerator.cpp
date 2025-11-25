#include "TerrainGenerator.h"
#include "PoissonSolver.h"
#include <Eigen/Dense>

TerrainGenerator::TerrainGenerator(int width, int height)
    : gradientField(width, height) {}

void TerrainGenerator::setGradient(int x, int y, double dx, double dy) {
    gradientField.setGradient(x, y, dx, dy);
}

Eigen::MatrixXd TerrainGenerator::generate() {
    PoissonSolver solver(gradientField);
    return solver.solve();
}
