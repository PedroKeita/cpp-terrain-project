#include "PoissonSolver.h"
#include <iostream>

PoissonSolver::PoissonSolver(const GradientField& grad) : gradient(grad) {}

Eigen::MatrixXd PoissonSolver::solve() {
    int w = gradient.width();
    int h = gradient.height();
    Eigen::MatrixXd height = Eigen::MatrixXd::Zero(h, w);

    std::cout << "DEBUG: PoissonSolver solving for " << w << "x" << h << std::endl;

    // Integração mais robusta - múltiplas passes
    for (int pass = 0; pass < 3; ++pass) {
        for(int y = 0; y < h; ++y) {
            for(int x = 0; x < w; ++x) {
                double sum = 0.0;
                int count = 0;

                if (x > 0) {
                    sum += height(y, x-1) + gradient.getGradient(x-1, y)[0];
                    count++;
                }
                if (y > 0) {
                    sum += height(y-1, x) + gradient.getGradient(x, y-1)[1];
                    count++;
                }
                if (x < w-1) {
                    sum += height(y, x+1) - gradient.getGradient(x, y)[0];
                    count++;
                }
                if (y < h-1) {
                    sum += height(y+1, x) - gradient.getGradient(x, y)[1];
                    count++;
                }

                if (count > 0) {
                    height(y, x) = sum / count;
                }
            }
        }
    }

    std::cout << "DEBUG: PoissonSolver finished - Min: " << height.minCoeff()
              << " Max: " << height.maxCoeff() << std::endl;

    return height;
}
