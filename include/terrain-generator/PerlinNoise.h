/**
* @file PerlinNoise.h
 * @brief Ken Perlin's improved noise algorithm implementation
 */
#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>

/**
 * @class PerlinNoise
 * @brief Generates 3D Perlin noise with fractal octave support
 *
 * Deterministic noise generation useful for procedural terrain,
 * textures, and natural effects.
 */
class PerlinNoise {
private:
    std::vector<int> p;  ///< 512-element permutation table

public:
    /** @brief Classic Ken Perlin permutation */
    PerlinNoise();

    /** @brief Seed-based random permutation */
    explicit PerlinNoise(unsigned int seed);

    /**
     * @brief 3D Perlin noise value
     * @return Value in range [-1.0, 1.0], periodic every 256 units
     */
    double noise(double x, double y, double z = 0.0);

    /**
     * @brief Multi-octave fractal noise
     * @param octaves Number of layers (4-8 typical)
     * @param persistence Amplitude multiplier per octave (0.0-1.0)
     */
    double octaveNoise(double x, double y, int octaves, double persistence);

private:
    double fade(double t);                       ///< 6t⁵-15t⁴+10t³ smoothing
    double lerp(double t, double a, double b);   ///< Linear interpolation
    double grad(int hash, double x, double y, double z); ///< Gradient dot product

    // Non-copyable
    PerlinNoise(const PerlinNoise&) = delete;
    PerlinNoise& operator=(const PerlinNoise&) = delete;
};

#endif