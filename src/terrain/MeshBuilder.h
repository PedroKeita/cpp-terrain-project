#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <Eigen/Dense>
#include <vector>

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

class MeshBuilder {
public:
    static void buildMesh(const Eigen::MatrixXd& heights,
                          std::vector<Vertex>& vertices,
                          std::vector<Face>& faces);
};

#endif
