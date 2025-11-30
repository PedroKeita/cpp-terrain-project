#include "TerrainGenerator.h"
#include "PoissonSolver.h"
#include <Eigen/Dense>
#include <iostream>

TerrainGenerator::TerrainGenerator(int width, int height, GradientField* field) {
    if (field)
        gradientField = *field;
    else
        gradientField = GradientField(width, height);
}

void TerrainGenerator::setGradient(int x, int y, double dx, double dy) {
    gradientField.setGradient(x, y, dx, dy);
}

Eigen::MatrixXd TerrainGenerator::generate() {
    std::cout << "DEBUG: Using REAL PoissonSolver for terrain generation" << std::endl;

    PoissonSolver solver(gradientField);
    Eigen::MatrixXd height = solver.solve();

    std::cout << "DEBUG: Real PoissonSolver result - Min: " << height.minCoeff()
              << " Max: " << height.maxCoeff() << std::endl;

    return height;
}
