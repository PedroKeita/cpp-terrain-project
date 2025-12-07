#pragma once
#include <Eigen/Core>
#include <vector>

class PerlinNoise {
private:
    std::vector<int> p;

public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y, double z = 0.0);
    double octaveNoise(double x, double y, int octaves, double persistence);

private:
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};