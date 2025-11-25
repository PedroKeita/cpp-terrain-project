#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <Eigen/Dense>
#include "GradientField.h"
#include "PoissonSolver.h"

class TerrainGenerator {
public:
    TerrainGenerator(int width = 0, int height = 0, GradientField* field = nullptr);

    void setGradient(int x, int y, double dx, double dy);
    Eigen::MatrixXd generate();

private:
    GradientField gradientField;
};

#endif
