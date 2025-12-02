#ifndef SHOW_TERRAIN_H
#define SHOW_TERRAIN_H

#include "../external/libigl/include/igl/opengl/glfw/Viewer.h"
#include <Eigen/Core>

inline void showTerrain(const Eigen::MatrixXd& heights)
{
    int h = heights.rows();
    int w = heights.cols();

    // --- Construção dos vértices ---
    Eigen::MatrixXd V(w * h, 3);
    int k = 0;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            V.row(k++) << x, y, heights(y, x);

    // --- Construção dos triângulos ---
    Eigen::MatrixXi F((w - 1) * (h - 1) * 2, 3);
    int t = 0;
    for (int y = 0; y < h - 1; ++y) {
        for (int x = 0; x < w - 1; ++x) {

            int i = y * w + x;

            F.row(t++) << i, i + 1, i + w;
            F.row(t++) << i + 1, i + w + 1, i + w;
        }
    }

    // ---- Viewer libigl ----
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);

    viewer.launch();
}

#endif
