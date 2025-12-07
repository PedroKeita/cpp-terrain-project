#pragma once
#include <QImage>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>

class ImageToGradient {
public:
    static void convert(const QImage& img, Eigen::MatrixXd& dx, Eigen::MatrixXd& dy) {
        int w = img.width();
        int h = img.height();

        cv::Mat gray(h, w, CV_8UC1);
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                QColor c = img.pixelColor(x, y);
                gray.at<uchar>(y, x) = static_cast<uchar>(0.299 * c.red() + 0.587 * c.green() + 0.114 * c.blue());
            }
        }

        cv::Mat gradX, gradY;
        cv::Sobel(gray, gradX, CV_64F, 1, 0, 3);
        cv::Sobel(gray, gradY, CV_64F, 0, 1, 3);

        dx.resize(h, w);
        dy.resize(h, w);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                dx(y, x) = gradX.at<double>(y, x);
                dy(y, x) = gradY.at<double>(y, x);
            }
        }
    }
};
