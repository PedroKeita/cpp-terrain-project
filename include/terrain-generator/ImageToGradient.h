#ifndef IMAGETOGRADIENT_H
#define IMAGETOGRADIENT_H

#include <QImage>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>
/**
 * @class ImageToGradient
 * @brief Converts QImage to gradient matrices using Sobel operator
 *
 * This class provides static methods to compute gradient fields
 * from QImage objects using OpenCV's Sobel operator.
 */
class ImageToGradient {
public:
    /**
     * @brief Convert QImage to gradient matrices
     *
     * @param img Input image (will be converted to grayscale)
     * @param dx Output matrix for gradient in X direction
     * @param dy Output matrix for gradient in Y direction
     *
     * @note The input image is converted to grayscale using
     *       luminance formula: 0.299*R + 0.587*G + 0.114*B
     * @note Uses 3x3 Sobel kernel for gradient computation
     * @note Output matrices are resized to match image dimensions
     */
    static void convert(const QImage& img, Eigen::MatrixXd& dx, Eigen::MatrixXd& dy);

    /**
     * @brief Alternative conversion with configurable parameters
     *
     * @param img Input image
     * @param dx Output gradient X
     * @param dy Output gradient Y
     * @param kernelSize Sobel kernel size (1, 3, 5, or 7)
     * @param scale Scale factor for derivatives
     * @param delta Delta added to derivatives
     */
    static void convertWithParams(const QImage& img,
                                  Eigen::MatrixXd& dx,
                                  Eigen::MatrixXd& dy,
                                  int kernelSize = 3,
                                  double scale = 1.0,
                                  double delta = 0.0);

    /**
     * @brief Compute gradient magnitude
     *
     * @param dx Gradient in X direction
     * @param dy Gradient in Y direction
     * @return Matrix of gradient magnitudes
     */
    static Eigen::MatrixXd computeMagnitude(const Eigen::MatrixXd& dx,
                                            const Eigen::MatrixXd& dy);

    static Eigen::MatrixXd computeDirection(const Eigen::MatrixXd& dx,
                                            const Eigen::MatrixXd& dy);

private:
    // Helper method for grayscale conversion
    static cv::Mat convertToGrayscale(const QImage& img);

    // Helper method for QImage to cv::Mat conversion
    static cv::Mat qImageToCvMat(const QImage& img);
};

#endif