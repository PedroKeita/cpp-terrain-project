#include "PoissonSolver.h"

PoissonSolver::PoissonSolver(const GradientField& grad) : gradient(grad) {}

Eigen::MatrixXd PoissonSolver::solve() {
    int w = gradient.getWidth();
    int h = gradient.getHeight();

    Eigen::MatrixXd height = Eigen::MatrixXd::Zero(h, w);

    for(int y = 1; y < h; ++y)
        height(y, 0) = height(y-1, 0) + gradient.getGradient(0, y)[1];

    for(int x = 1; x < w; ++x)
        height(0, x) = height(0, x-1) + gradient.getGradient(x, 0)[0];

    for(int y = 1; y < h; ++y)
        for(int x = 1; x < w; ++x)
        {
            double fromLeft  = height(y, x-1) + gradient.getGradient(x, y)[0];
            double fromTop   = height(y-1, x) + gradient.getGradient(x, y)[1];
            height(y, x) = (fromLeft + fromTop) * 0.5;
        }

    return height;
}

