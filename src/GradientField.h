#pragma once
#include <Eigen/Core>

class GradientField {
public:
    GradientField(const Eigen::MatrixXd& dx, const Eigen::MatrixXd& dy)
        : m_dx(dx), m_dy(dy) {}

    const Eigen::MatrixXd& dx() const { return m_dx; }
    const Eigen::MatrixXd& dy() const { return m_dy; }

private:
    Eigen::MatrixXd m_dx, m_dy;
};
