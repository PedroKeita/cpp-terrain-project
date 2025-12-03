#ifndef SHOW_TERRAIN_H
#define SHOW_TERRAIN_H

#include "../external/libigl/include/igl/opengl/glfw/Viewer.h"
#include <Eigen/Core>
#include <cmath>

inline void showTerrain(const Eigen::MatrixXd& heights)
{
    int h = heights.rows();
    int w = heights.cols();

    // Construcao dos vertices com escala adequada
    Eigen::MatrixXd V(w * h, 3);
    int k = 0;
    double scale = 1.0 / std::max(w, h);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            V.row(k++) << (x - w/2.0) * scale,
                          (y - h/2.0) * scale,
                          heights(y, x) * scale * 0.5;
        }
    }

    // Construcao dos triangulos
    Eigen::MatrixXi F((w - 1) * (h - 1) * 2, 3);
    int t = 0;
    for (int y = 0; y < h - 1; ++y) {
        for (int x = 0; x < w - 1; ++x) {
            int i = y * w + x;
            F.row(t++) << i, i + 1, i + w;
            F.row(t++) << i + 1, i + w + 1, i + w;
        }
    }

    // Viewer libigl
    igl::opengl::glfw::Viewer viewer;

    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);

    // Cores baseadas na altura - gradiente realista de terreno
    Eigen::MatrixXd C(F.rows(), 3);
    double minH = heights.minCoeff();
    double maxH = heights.maxCoeff();
    double range = maxH - minH;
    if (range < 1e-6) range = 1.0;

    for (int i = 0; i < F.rows(); ++i) {
        double avgH = 0;
        for (int j = 0; j < 3; ++j) {
            int vidx = F(i, j);
            int vy = vidx / w;
            int vx = vidx % w;
            avgH += heights(vy, vx);
        }
        avgH /= 3.0;
        double t = (avgH - minH) / range;

        // Gradiente: agua azul -> areia -> grama verde -> terra marrom -> rocha cinza -> neve branca
        if (t < 0.1) {
            // Agua azul profundo
            C(i, 0) = 0.1;
            C(i, 1) = 0.3;
            C(i, 2) = 0.6;
        } else if (t < 0.2) {
            // Areia/praia
            double s = (t - 0.1) / 0.1;
            C(i, 0) = 0.1 + s * 0.7;
            C(i, 1) = 0.3 + s * 0.5;
            C(i, 2) = 0.6 - s * 0.3;
        } else if (t < 0.5) {
            // Grama verde
            double s = (t - 0.2) / 0.3;
            C(i, 0) = 0.2 - s * 0.05;
            C(i, 1) = 0.55 + s * 0.15;
            C(i, 2) = 0.15 + s * 0.05;
        } else if (t < 0.7) {
            // Terra/floresta escura
            double s = (t - 0.5) / 0.2;
            C(i, 0) = 0.15 + s * 0.25;
            C(i, 1) = 0.7 - s * 0.35;
            C(i, 2) = 0.2 - s * 0.05;
        } else if (t < 0.85) {
            // Rocha cinza
            double s = (t - 0.7) / 0.15;
            C(i, 0) = 0.4 + s * 0.15;
            C(i, 1) = 0.35 + s * 0.15;
            C(i, 2) = 0.15 + s * 0.35;
        } else {
            // Neve branca
            double s = (t - 0.85) / 0.15;
            C(i, 0) = 0.55 + s * 0.4;
            C(i, 1) = 0.5 + s * 0.45;
            C(i, 2) = 0.5 + s * 0.45;
        }
    }
    viewer.data().set_colors(C);

    // Configuracoes de visualizacao
    viewer.data().show_lines = false;
    viewer.core().is_animating = false;
    viewer.core().background_color << 0.05f, 0.05f, 0.05f, 1.0f;

    // Camera inicial
    viewer.core().camera_zoom = 1.5f;

    viewer.launch();
}

#endif
