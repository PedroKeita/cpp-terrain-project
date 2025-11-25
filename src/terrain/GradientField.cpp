#include "GradientField.h"

GradientField::GradientField(int width, int height)
    : width_(width), height_(height),
    fieldX(Eigen::MatrixXd::Zero(height, width)),
    fieldY(Eigen::MatrixXd::Zero(height, width)) {}

void GradientField::setGradient(int x, int y, double dx, double dy) {
    fieldX(y,x) = dx;
    fieldY(y,x) = dy;
}

Eigen::Vector2d GradientField::getGradient(int x, int y) const {
    return Eigen::Vector2d(fieldX(y,x), fieldY(y,x));
}

int GradientField::getWidth() const { return width_; }
int GradientField::getHeight() const { return height_; }
