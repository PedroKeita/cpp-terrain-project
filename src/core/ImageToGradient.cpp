#include "../../include/terrain-generator/ImageToGradient.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <stdexcept>

// Implementation of the main conversion method
void ImageToGradient::convert(const QImage& img, Eigen::MatrixXd& dx, Eigen::MatrixXd& dy) {
    // Validate input
    if (img.isNull()) {
        throw std::invalid_argument("ImageToGradient::convert: Input image is null");
    }
    
    int w = img.width();
    int h = img.height();
    
    // Convert to grayscale using OpenCV for efficiency
    cv::Mat gray = convertToGrayscale(img);
    
    // Compute gradients using Sobel operator
    cv::Mat gradX, gradY;
    cv::Sobel(gray, gradX, CV_64F, 1, 0, 3);
    cv::Sobel(gray, gradY, CV_64F, 0, 1, 3);
    
    // Resize and fill Eigen matrices
    dx.resize(h, w);
    dy.resize(h, w);
    
    // Copy data from OpenCV to Eigen
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            dx(y, x) = gradX.at<double>(y, x);
            dy(y, x) = gradY.at<double>(y, x);
        }
    }
}

// Alternative method with configurable parameters
void ImageToGradient::convertWithParams(const QImage& img, 
                                        Eigen::MatrixXd& dx, 
                                        Eigen::MatrixXd& dy,
                                        int kernelSize,
                                        double scale,
                                        double delta) {
    // Validate kernel size
    if (kernelSize != 1 && kernelSize != 3 && kernelSize != 5 && kernelSize != 7) {
        throw std::invalid_argument("ImageToGradient::convertWithParams: "
                                   "Kernel size must be 1, 3, 5, or 7");
    }
    
    if (img.isNull()) {
        throw std::invalid_argument("ImageToGradient::convertWithParams: "
                                   "Input image is null");
    }
    
    int w = img.width();
    int h = img.height();
    
    cv::Mat gray = convertToGrayscale(img);
    
    cv::Mat gradX, gradY;
    cv::Sobel(gray, gradX, CV_64F, 1, 0, kernelSize, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(gray, gradY, CV_64F, 0, 1, kernelSize, scale, delta, cv::BORDER_DEFAULT);
    
    dx.resize(h, w);
    dy.resize(h, w);
    
    // Use pointer arithmetic for better performance
    double* dxData = dx.data();
    double* dyData = dy.data();
    const double* gradXData = reinterpret_cast<const double*>(gradX.data);
    const double* gradYData = reinterpret_cast<const double*>(gradY.data);
    
    for (int i = 0; i < h * w; ++i) {
        dxData[i] = gradXData[i];
        dyData[i] = gradYData[i];
    }
}

// Compute gradient magnitude
Eigen::MatrixXd ImageToGradient::computeMagnitude(const Eigen::MatrixXd& dx, 
                                                  const Eigen::MatrixXd& dy) {
    // Validate input dimensions
    if (dx.rows() != dy.rows() || dx.cols() != dy.cols()) {
        throw std::invalid_argument("ImageToGradient::computeMagnitude: "
                                   "dx and dy matrices must have same dimensions");
    }
    
    int rows = dx.rows();
    int cols = dx.cols();
    
    Eigen::MatrixXd magnitude(rows, cols);
    
    // Compute magnitude for each pixel: sqrt(dx² + dy²)
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            double dxVal = dx(y, x);
            double dyVal = dy(y, x);
            magnitude(y, x) = std::sqrt(dxVal * dxVal + dyVal * dyVal);
        }
    }
    
    return magnitude;
}

// Compute gradient direction (angle)
Eigen::MatrixXd ImageToGradient::computeDirection(const Eigen::MatrixXd& dx, 
                                                  const Eigen::MatrixXd& dy) {
    // Validate input dimensions
    if (dx.rows() != dy.rows() || dx.cols() != dy.cols()) {
        throw std::invalid_argument("ImageToGradient::computeDirection: "
                                   "dx and dy matrices must have same dimensions");
    }
    
    int rows = dx.rows();
    int cols = dx.cols();
    
    Eigen::MatrixXd direction(rows, cols);
    
    // Compute direction for each pixel: atan2(dy, dx)
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            direction(y, x) = std::atan2(dy(y, x), dx(y, x));
        }
    }
    
    return direction;
}

// Private helper: Convert QImage to grayscale OpenCV Mat
cv::Mat ImageToGradient::convertToGrayscale(const QImage& img) {
    int w = img.width();
    int h = img.height();
    
    cv::Mat gray(h, w, CV_8UC1);
    
    // Manual conversion for maximum control
    if (img.format() == QImage::Format_RGB32 || 
        img.format() == QImage::Format_ARGB32) {
        
        for (int y = 0; y < h; ++y) {
            const QRgb* scanLine = reinterpret_cast<const QRgb*>(img.scanLine(y));
            uchar* grayLine = gray.ptr<uchar>(y);
            
            for (int x = 0; x < w; ++x) {
                QRgb pixel = scanLine[x];
                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);
                
                // Luminance formula
                grayLine[x] = static_cast<uchar>(0.299 * r + 0.587 * g + 0.114 * b);
            }
        }
    } else {
        // For other formats, use slower but general method
        for (int y = 0; y < h; ++y) {
            uchar* grayLine = gray.ptr<uchar>(y);
            for (int x = 0; x < w; ++x) {
                QColor c = img.pixelColor(x, y);
                grayLine[x] = static_cast<uchar>(0.299 * c.red() + 0.587 * c.green() + 0.114 * c.blue());
            }
        }
    }
    
    return gray;
}

// Private helper: Convert QImage to cv::Mat (optional, for other operations)
cv::Mat ImageToGradient::qImageToCvMat(const QImage& img) {
    // Convert QImage to appropriate OpenCV format
    switch (img.format()) {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            cv::Mat mat(img.height(), img.width(), CV_8UC4, 
                       const_cast<uchar*>(img.bits()), 
                       static_cast<size_t>(img.bytesPerLine()));
            return mat.clone(); // Clone to ensure data ownership
        }
        case QImage::Format_RGB888: {
            cv::Mat mat(img.height(), img.width(), CV_8UC3, 
                       const_cast<uchar*>(img.bits()), 
                       static_cast<size_t>(img.bytesPerLine()));
            cv::Mat rgb;
            cv::cvtColor(mat, rgb, cv::COLOR_RGB2BGR);
            return rgb;
        }
        case QImage::Format_Grayscale8: {
            cv::Mat mat(img.height(), img.width(), CV_8UC1, 
                       const_cast<uchar*>(img.bits()), 
                       static_cast<size_t>(img.bytesPerLine()));
            return mat.clone();
        }
        default: {
            // Convert to RGB32 format and try again
            QImage converted = img.convertToFormat(QImage::Format_RGB32);
            return qImageToCvMat(converted);
        }
    }
}