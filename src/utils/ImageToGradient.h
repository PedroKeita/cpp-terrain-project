#pragma once
#include <QImage>
#include <Eigen/Dense>

class ImageToGradient {
public:
    static void convert(const QImage &img,
                        Eigen::MatrixXd &dx,
                        Eigen::MatrixXd &dy);
};
