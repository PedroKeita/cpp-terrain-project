#ifndef GRADIENTFIELD_H
#define GRADIENTFIELD_H

#include <Eigen/Dense>

class GradientField {
public:
    GradientField(int width = 0, int height = 0);

    void setGradient(int x, int y, double dx, double dy);
    Eigen::Vector2d getGradient(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

private:
    int width_, height_;
    Eigen::MatrixXd fieldX;
    Eigen::MatrixXd fieldY;
};

#endif
