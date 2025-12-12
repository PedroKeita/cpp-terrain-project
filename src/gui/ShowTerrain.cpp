/**
 * @file ShowTerrain.cpp
 * @brief Implementation of function to generate and display 3D terrain using libigl.
 *
 * This module constructs a triangular mesh based on a height matrix
 * and displays it in an OpenGL viewer provided by the LibIGL library.
 */

#include "../../include/terrain-generator/ShowTerrain.h"
#include "../../external/libigl/include/igl/opengl/glfw/Viewer.h"
#include <Eigen/Core>
#include <cmath>

/**
 * @brief Displays a 3D mesh based on a heightmap.
 *
 * The function receives a height matrix (heights), converts it to a set of 3D vertices,
 * constructs triangulation to form a mesh, and applies coloring based on altitude.
 * Then uses LibIGL to render the terrain interactively.
 *
 * @param heights Matrix (Eigen::MatrixXd) containing terrain heights.
 *        Each element heights(y, x) represents elevation at point (x, y).
 *
 * @note The terrain is automatically scaled to fit the viewer.
 * @note Colors are interpolated based on the average height of each triangle.
 */
void showTerrain(const Eigen::MatrixXd& heights)
{
    int h = heights.rows(); ///< Grid height (number of rows)
    int w = heights.cols(); ///< Grid width (number of columns)

    // -------------------------------------------------------------------------
    // Vertex construction
    // -------------------------------------------------------------------------

    Eigen::MatrixXd V(w * h, 3); ///< Mesh vertex matrix
    int k = 0;

    /// Scale to normalize terrain and center it
    double scale = 1.0 / std::max(w, h);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            V.row(k++) << (x - w/2.0) * scale, // X position
                          (y - h/2.0) * scale, // Y position
                          heights(y, x) * scale * 0.5;  // Z height
        }
    }

    // -------------------------------------------------------------------------
    // Face construction (triangles)
    // -------------------------------------------------------------------------

    Eigen::MatrixXi F((w - 1) * (h - 1) * 2, 3); ///< Mesh connectivity
    int t = 0;

    for (int y = 0; y < h - 1; ++y) {
        for (int x = 0; x < w - 1; ++x) {
            int i = y * w + x;
            F.row(t++) << i, i + 1, i + w;  //  1st triangle of cell
            F.row(t++) << i + 1, i + w + 1, i + w; // 2nd triangle
        }
    }

    // -------------------------------------------------------------------------
    // LibIGL viewer initialization
    // -------------------------------------------------------------------------

    igl::opengl::glfw::Viewer viewer;

    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);

    // -------------------------------------------------------------------------
    // Triangle coloring based on height
    // -------------------------------------------------------------------------

    Eigen::MatrixXd C(F.rows(), 3); ///< Colors assigned to each face

    double minH = heights.minCoeff();
    double maxH = heights.maxCoeff();
    double range = maxH - minH;

    /// Avoid division by zero if all points have the same height
    if (range < 1e-6) range = 1.0;

    // Calculate triangle average height
    for (int i = 0; i < F.rows(); ++i) {
        double avgH = 0;
        for (int j = 0; j < 3; ++j) {
            int vidx = F(i, j);
            int vy = vidx / w;
            int vx = vidx % w;
            avgH += heights(vy, vx);
        }
        avgH /= 3.0;

        double t = (avgH - minH) / range; ///< Normalization (0 to 1)

        // Color gradient (earth → green → rock → snow)
        if (t < 0.1) {
            C(i, 0) = 0.1;
            C(i, 1) = 0.3;
            C(i, 2) = 0.6;
        }

        else if (t < 0.2) {
            double s = (t - 0.1) / 0.1;
            C(i, 0) = 0.1 + s * 0.7;
            C(i, 1) = 0.3 + s * 0.5;
            C(i, 2) = 0.6 - s * 0.3;
        }

        else if (t < 0.5) {
            double s = (t - 0.2) / 0.3;
            C(i, 0) = 0.2 - s * 0.05;
            C(i, 1) = 0.55 + s * 0.15;
            C(i, 2) = 0.15 + s * 0.05;
        }

        else if (t < 0.7) {
            double s = (t - 0.5) / 0.2;
            C(i, 0) = 0.15 + s * 0.25;
            C(i, 1) = 0.7 - s * 0.35;
            C(i, 2) = 0.2 - s * 0.05;
        }

        else if (t < 0.85) {
            double s = (t - 0.7) / 0.15;
            C(i, 0) = 0.4 + s * 0.15;
            C(i, 1) = 0.35 + s * 0.15;
            C(i, 2) = 0.15 + s * 0.35;
        }

        else {
            double s = (t - 0.85) / 0.15;
            C(i, 0) = 0.55 + s * 0.4;
            C(i, 1) = 0.5 + s * 0.45;
            C(i, 2) = 0.5 + s * 0.45;
        }
    }
    viewer.data().set_colors(C);

    // Viewer visual settings
    viewer.data().show_lines = false;
    viewer.core().is_animating = false;
    viewer.core().background_color << 0.05f, 0.05f, 0.05f, 1.0f;
    viewer.core().camera_zoom = 1.5f;

    viewer.launch();
}