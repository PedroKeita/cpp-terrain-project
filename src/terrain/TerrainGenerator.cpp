#include "TerrainGenerator.h"
#include "PoissonSolver.h"
#include <Eigen/Dense>


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
    PoissonSolver solver(gradientField);
    return solver.solve();
}
