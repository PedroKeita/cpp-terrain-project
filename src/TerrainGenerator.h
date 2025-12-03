#pragma once
#include <Eigen/Core>
#include <Eigen/Sparse>

class TerrainGenerator {
public:
    TerrainGenerator(const Eigen::MatrixXd& dx, const Eigen::MatrixXd& dy)
        : m_dx(dx), m_dy(dy) {}

    Eigen::MatrixXd generate();

private:
    Eigen::MatrixXd m_dx, m_dy;
};
