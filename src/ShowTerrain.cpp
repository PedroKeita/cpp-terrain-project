/**
 * @file ShowTerrain.cpp
 * @brief Implementação da função responsável por gerar e exibir um terreno 3D usando libigl.
 *
 * Este módulo constrói uma malha triangular baseada em uma matriz de alturas
 * e a exibe em um viewer OpenGL fornecido pela biblioteca LibIGL.
 */

#include "ShowTerrain.h"
#include "../external/libigl/include/igl/opengl/glfw/Viewer.h"
#include <Eigen/Core>
#include <cmath>

/**
 * @brief Exibe uma malha 3D baseada em um mapa de alturas.
 *
 * A função recebe uma matriz de alturas (heights), converte-a em um conjunto de vértices 3D,
 * constrói triangulação para formar uma malha e aplica coloração baseada na altitude.
 * Em seguida, utiliza o LibIGL para renderizar o terreno interativamente.
 *
 * @param heights Matriz (Eigen::MatrixXd) contendo as alturas do terreno.
 *        Cada elemento heights(y, x) representa a elevação no ponto (x, y).
 *
 * @note O terreno é escalonado automaticamente para caber no viewer.
 * @note As cores são interpoladas com base na altura média de cada triângulo.
 */
void showTerrain(const Eigen::MatrixXd& heights)
{
    int h = heights.rows(); ///< Altura da grade (número de linhas)
    int w = heights.cols(); ///< Largura da grade (número de colunas)

    // -------------------------------------------------------------------------
    // Construção dos vértices
    // -------------------------------------------------------------------------

    Eigen::MatrixXd V(w * h, 3); ///< Matriz de vértices da malha
    int k = 0;

    /// Escala para normalizar o terreno e centralizá-lo
    double scale = 1.0 / std::max(w, h);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            V.row(k++) << (x - w/2.0) * scale, // posição X
                          (y - h/2.0) * scale, // posição Y
                          heights(y, x) * scale * 0.5;  // Altura Z
        }
    }

    // -------------------------------------------------------------------------
    // Construção das faces (triângulos)
    // -------------------------------------------------------------------------

    Eigen::MatrixXi F((w - 1) * (h - 1) * 2, 3); ///< Conectividade da malha
    int t = 0;

    for (int y = 0; y < h - 1; ++y) {
        for (int x = 0; x < w - 1; ++x) {
            int i = y * w + x;
            F.row(t++) << i, i + 1, i + w;  //  1º triângulo da célula
            F.row(t++) << i + 1, i + w + 1, i + w; // 2º triângulo
        }
    }

    // -------------------------------------------------------------------------
    // Inicialização do viewer LibIGL
    // -------------------------------------------------------------------------

    igl::opengl::glfw::Viewer viewer;

    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);

    // -------------------------------------------------------------------------
    // Coloração dos triângulos baseada na altura
    // -------------------------------------------------------------------------

    Eigen::MatrixXd C(F.rows(), 3); ///< Cores atribuidas a cada face

    double minH = heights.minCoeff();
    double maxH = heights.maxCoeff();
    double range = maxH - minH;

    /// Evita divisão por zero caso todos os pontos tenham a mesma altura
    if (range < 1e-6) range = 1.0;


    // Calcula a altura média do triângulo
    for (int i = 0; i < F.rows(); ++i) {
        double avgH = 0;
        for (int j = 0; j < 3; ++j) {
            int vidx = F(i, j);
            int vy = vidx / w;
            int vx = vidx % w;
            avgH += heights(vy, vx);
        }
        avgH /= 3.0;

        double t = (avgH - minH) / range; ///< Normalização (0 a 1)

        // Gradiente de cores (terra → verde → rocha → neve)
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

    // Configurações visuais do viewer
    viewer.data().show_lines = false;
    viewer.core().is_animating = false;
    viewer.core().background_color << 0.05f, 0.05f, 0.05f, 1.0f;
    viewer.core().camera_zoom = 1.5f;

    viewer.launch();
}
