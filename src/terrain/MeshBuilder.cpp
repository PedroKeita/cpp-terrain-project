#include "MeshBuilder.h"

void MeshBuilder::buildMesh(const Eigen::MatrixXd& heights,
                            std::vector<Vertex>& vertices,
                            std::vector<Face>& faces)
{
    int rows = heights.rows();
    int cols = heights.cols();

    vertices.clear();
    faces.clear();

    vertices.reserve(rows * cols); // Uso do reserve() melhora performance, evitando realocação
    faces.reserve((rows - 1) * (cols - 1) * 2);

    // Criar vértices
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            vertices.push_back(Vertex{
                float(x),
                float(y),
                float(heights(y, x))
            });
        }
    }

    // Criar faces (2 triângulos por célula da grade)
    for (int y = 0; y < rows - 1; ++y) {
        for (int x = 0; x < cols - 1; ++x) {

            int a = y * cols + x;
            int b = a + 1;
            int c = a + cols;
            int d = c + 1;

            // Primeiro triângulo
            faces.push_back({a, b, c});

            // Segundo triângulo
            faces.push_back({b, d, c});
        }
    }
}
