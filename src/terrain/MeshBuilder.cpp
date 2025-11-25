#include "MeshBuilder.h"

void MeshBuilder::buildMesh(const Eigen::MatrixXd& heights,
                            std::vector<Vertex>& vertices,
                            std::vector<Face>& faces) {
    int rows = heights.rows();
    int cols = heights.cols();

    vertices.clear();
    faces.clear();

    // Criar vertices
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            vertices.push_back({float(x), float(y), float(heights(y, x))});
        }
    }

    // Criar faces (triângulos)
    for (int y = 0; y < rows - 1; ++y) {
        for (int x = 0; x < cols - 1; ++x) {
            int i = y * cols + x;
            faces.push_back({i, i + 1, i + cols});
            faces.push_back({i + 1, i + 1 + cols, i + cols});
        }
    }
}
