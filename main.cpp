#include <iostream>
#include "TerrainGenerator.h"
#include "MeshBuilder.h"

int main() {
    // Dimensões do terreno
    int width = 5;
    int height = 5;

    // Criar o gerador de terreno
    TerrainGenerator terrainGen(width, height);

    // Definir gradientes manualmente
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double dx = x * 0.1;   // exemplo de derivada x
            double dy = y * 0.05;  // exemplo de derivada y
            terrainGen.setGradient(x, y, dx, dy);
        }
    }

    // Gerar o mapa de alturas
    Eigen::MatrixXd heightMap = terrainGen.generate();

    std::cout << "Mapa de alturas gerado:\n" << heightMap << "\n\n";

    // Construir a malha
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    MeshBuilder::buildMesh(heightMap, vertices, faces);

    std::cout << "Vertices gerados: " << vertices.size() << "\n";
    std::cout << "Faces geradas: " << faces.size() << "\n";

    // Imprimir alguns vertices e faces de exemplo
    for (size_t i = 0; i < vertices.size() && i < 5; ++i) {
        std::cout << "Vertex " << i << ": ("
                  << vertices[i].x << ", "
                  << vertices[i].y << ", "
                  << vertices[i].z << ")\n";
    }

    for (size_t i = 0; i < faces.size() && i < 5; ++i) {
        std::cout << "Face " << i << ": ("
                  << faces[i].v1 << ", "
                  << faces[i].v2 << ", "
                  << faces[i].v3 << ")\n";
    }

    return 0;
}
