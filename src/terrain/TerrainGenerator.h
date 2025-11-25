#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <Eigen/Dense>
#include "GradientField.h"

class TerrainGenerator {
public:
    TerrainGenerator(int width = 0, int height = 0);

    void setGradient(int x, int y, double dx, double dy);
    Eigen::MatrixXd generate();

private:
    GradientField gradientField;
};

#endif
