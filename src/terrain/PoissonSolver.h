#ifndef POISSONSOLVER_H
#define POISSONSOLVER_H

#include "GradientField.h"
#include <Eigen/Dense>

class PoissonSolver {
public:
    PoissonSolver(const GradientField& grad);

    Eigen::MatrixXd solve();

private:
    const GradientField& gradient;
};

#endif
