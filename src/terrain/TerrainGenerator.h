#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <Eigen/Dense>
#include "GradientField.h"

class TerrainGenerator {
public:
    TerrainGenerator(int width, int height, GradientField* field);
    void setGradient(int x, int y, double dx, double dy);
    Eigen::MatrixXd generate();

private:
    GradientField gradientField;
};

#endif // TERRAINGENERATOR_H
