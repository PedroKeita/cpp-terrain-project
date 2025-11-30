#include "ImageToGradient.h"
#include <QColor>

void ImageToGradient::convert(const QImage &img,
                              Eigen::MatrixXd &dx,
                              Eigen::MatrixXd &dy)
{
    int w = img.width();
    int h = img.height();

    // Eigen: rows = h, cols = w
    dx = Eigen::MatrixXd::Zero(h, w);
    dy = Eigen::MatrixXd::Zero(h, w);

    auto gray = [&](int x, int y) -> double {
        QColor c(img.pixel(x, y));
        return c.red() * 0.3 + c.green() * 0.59 + c.blue() * 0.11;
    };

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            double gx =
                -gray(x-1,y-1) - 2*gray(x-1,y) - gray(x-1,y+1)
                +gray(x+1,y-1) + 2*gray(x+1,y) + gray(x+1,y+1);

            double gy =
                -gray(x-1,y-1) - 2*gray(x,  y-1) - gray(x+1,y-1)
                +gray(x-1,y+1) + 2*gray(x,  y+1) + gray(x+1,y+1);

            dx(y,x) = gx;
            dy(y,x) = gy;
        }
    }

    double maxGrad = 0;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            maxGrad = std::max(maxGrad, std::max(std::abs(dx(y,x)), std::abs(dy(y,x))));
        }
    }

    if (maxGrad > 0) {
        dx /= maxGrad;
        dy /= maxGrad;
    }
}
