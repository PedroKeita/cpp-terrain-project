/**
* @file PerlinNoise.cpp
 * @brief Implementação da classe PerlinNoise para geração de ruído suave 3D.
 *
 * Esta classe implementa o algoritmo clássico de Perlin Noise, criado por Ken Perlin.
 * O ruído gerado é contínuo, suave e amplamente utilizado em:
 *  - Geração de terrenos
 *  - Efeitos naturais (nuvens, fogo, água)
 *  - Sombreamento procedural
 *  - Texturas geradas por programação
 *
 * A classe suporta ruído clássico 3D e ruído multi-oitavas.
 */

#include "PerlinNoise.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>

/**
 * @brief Construtor padrão usando a permutação clássica de Ken Perlin.
 *
 * Esta tabela de permutação produz um ruído determinístico padrão.
 * O vetor interno `p` contém 512 elementos onde os primeiros 256 são
 * duplicados no final, facilitando operações com wrap-around.
 */
PerlinNoise::PerlinNoise() {
    // Permutação padrão de Ken Perlin
    std::vector<int> permutation = {
        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,
        140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,
        247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,
        57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,
        175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,
        229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,
        89,18,169,200,196,135,130,116,188,159,86,164,100,109,
        198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,
        118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
        189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
        221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,
        110,79,113,224,232,178,185,112,104,218,246,97,228,251,
        34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,
        249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,
            176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,
            67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

    p.resize(512);
    for (int i = 0; i < 256; i++) {
        p[256 + i] = p[i] = permutation[i];
    }
}

/**
 * @brief Construtor que utiliza uma semente (seed) para gerar permutação aleatória.
 *
 * @param seed Valor para inicializar o gerador determinístico.
 *
 * Este método cria uma variação diferente do ruído puramente baseada na semente,
 * permitindo geração procedural reprodutível.
 */
PerlinNoise::PerlinNoise(unsigned int seed) {
    p.resize(512);
    std::vector<int> permutation(256);
    std::iota(permutation.begin(), permutation.end(), 0);

    std::default_random_engine engine(seed);
    std::shuffle(permutation.begin(), permutation.end(), engine);

    for (int i = 0; i < 256; i++) {
        p[256 + i] = p[i] = permutation[i];
    }
}

/**
 * @brief Função de suavização (fade) usada por Ken Perlin.
 *
 * A função:
 *      6t⁵ - 15t⁴ + 10t³
 *
 * fornece suavidade C2 (continuidade de segunda derivada),
 * essencial para eliminar artefatos.
 */
double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}


/**
 * @brief Interpolação linear (lerp).
 *
 * @param t Parâmetro de interpolação (0 → a, 1 → b)
 * @param a Valor inicial
 * @param b Valor final
 */
double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

/**
 * @brief Calcula o dot-product entre o gradiente pseudo-aleatório e o vetor deslocamento.
 *
 * @param hash Hash do gradiente gerado pela permutação
 * @param x offset X
 * @param y offset Y
 * @param z offset Z
 *
 * A escolha dos gradientes depende dos 4 bits mais baixos do hash.
 */
double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

/**
 * @brief Calcula o Perlin Noise 3D.
 *
 * A função segue a implementação original combinando:
 *  - Gradientes
 *  - Fade curves
 *  - Interpolação trilinear
 *
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param z Coordenada Z
 *
 * @return Valor do ruído entre [-1, 1]
 */
double PerlinNoise::noise(double x, double y, double z) {

    // Localiza células de grade
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;

    // Distâncias internas
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Curvas de suavização
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    // Hashes das 8 esquinas da célula
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    // Interpolação trilinear completa
    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                   grad(p[BA], x - 1, y, z)),
                           lerp(u, grad(p[AB], x, y - 1, z),
                                   grad(p[BB], x - 1, y - 1, z))),
                   lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                   grad(p[BA + 1], x - 1, y, z - 1)),
                           lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                                   grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

/**
 * @brief Gera Perlin Noise de múltiplas oitavas (fractal noise).
 *
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param octaves Número de oitavas (camadas)
 * @param persistence Redução da amplitude a cada oitava (0–1)
 *
 * O ruído final é uma soma ponderada de várias camadas:
 *
 *      total += noise(x * freq) * amp
 *
 * @return Valor normalizado do ruído em [-1, 1]
 */
double PerlinNoise::octaveNoise(double x, double y, int octaves, double persistence) {
    double total = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    double maxValue = 0.0;
    
    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency, y * frequency, 0.0) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }
    
    return total / maxValue;
}