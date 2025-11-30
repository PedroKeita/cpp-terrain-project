#pragma once

#include <Eigen/Dense>
#include <vector>

struct Vertex {   // --> representa uma vértice (considere um ponto) no espaço 3D
    float x, y, z; // -> Aqui são as 3 coordenadas, x -> horizontal, y -> vertical, z -> altura
};

struct Face { // --> representa um triângulo da malha
    int v1, v2, v3;
};

class MeshBuilder {
public:   // --> Classe util estática que vai gerar uma malha(mesh) a partir da matriz de alturas
    static void buildMesh(const Eigen::MatrixXd& heights,
                          std::vector<Vertex>& vertices,
                          std::vector<Face>& faces);
};


