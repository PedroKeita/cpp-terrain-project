/**
* @file TerrainGenerator.cpp
 * @brief Implementação da classe TerrainGenerator responsável por reconstruir
 *        uma superfície a partir de um campo de gradientes usando a equação de Poisson.
 *
 * O algoritmo integra um campo de derivadas parciais m_dx e m_dy reconstruindo a
 * superfície original Z(x, y) ao resolver a equação:
 *
 *      ∆Z = div(gradient field)
 *
 * onde o lado direito é calculado através das diferenças finitas do gradiente.
 */

#include "TerrainGenerator.h"
#include <Eigen/IterativeLinearSolvers>

/**
 * @brief Gera uma matriz de alturas resolvendo a equação de Poisson para o campo de gradientes.
 *
 * A função reconstrói o mapa de alturas a partir dos campos de derivadas m_dx e m_dy.
 * O processo funciona em três etapas:
 *
 *  1. Montagem do vetor b contendo a divergência do campo de gradiente.
 *  2. Construção da matriz Laplaciana discreta usando diferenças finitas.
 *  3. Resolução do sistema L * Z = b usando método de Conjugate Gradient.
 *
 * @return Eigen::MatrixXd Matriz (h × w) com as alturas reconstruídas.
 *
 * @note Este método resolve um sistema esparso de grande porte, portanto é eficiente
 *       para grandes terrenos.
 */
Eigen::MatrixXd TerrainGenerator::generate() {
    int h = m_dx.rows(); ///< Quantidade de linhas do mapa
    int w = m_dx.cols(); ///< Quantidade de colunas do mapa
    int n = h * w; ///< Quantidade total de variáveis do sistema

    // -------------------------------------------------------------------------
    //  Montar o vetor 'b' contendo a divergência do campo de gradiente
    // -------------------------------------------------------------------------


    Eigen::VectorXd b(n);
    b.setZero();

    /**
     * A divergência é calculada através de diferenças finitas:
     *
     * div = d/dx(dx) + d/dy(dy)
     *
     * com tratamento especial para bordas.
     */
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int i = y * w + x;
            double divX = 0, divY = 0;

            // ----- d/dx -----
            if (x > 0 && x < w - 1)
                divX = (m_dx(y, x + 1) - m_dx(y, x - 1)) / 2.0;
            else if (x == 0)
                divX = m_dx(y, x + 1) - m_dx(y, x);
            else
                divX = m_dx(y, x) - m_dx(y, x - 1);

            // ----- d/dy -----
            if (y > 0 && y < h - 1)
                divY = (m_dy(y + 1, x) - m_dy(y - 1, x)) / 2.0;
            else if (y == 0)
                divY = m_dy(y + 1, x) - m_dy(y, x);
            else
                divY = m_dy(y, x) - m_dy(y - 1, x);

            b(i) = divX + divY;
        }
    }

    // -------------------------------------------------------------------------
    // Construção da matriz Laplaciana esparsa (5-diagonal)
    // -------------------------------------------------------------------------

    typedef Eigen::Triplet<double> T;
    std::vector<T> triplets;
    triplets.reserve(5 * n); ///< Cada célula contribui com até 5 valores


    /**
     * Para cada célula, conectamos:
     *
     *  - Esquerda
     *  - Direita
     *  - Cima
     *  - Baixo
     *
     * E no próprio índice colocamos o valor negativo da quantidade de vizinhos:
     *
     *      L(i,i) = -deg(i)
     */
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int i = y * w + x;
            int neighbors = 0;

            if (x > 0) { triplets.push_back(T(i, i - 1, 1.0)); neighbors++; }
            if (x < w - 1) { triplets.push_back(T(i, i + 1, 1.0)); neighbors++; }
            if (y > 0) { triplets.push_back(T(i, i - w, 1.0)); neighbors++; }
            if (y < h - 1) { triplets.push_back(T(i, i + w, 1.0)); neighbors++; }

            triplets.push_back(T(i, i, -static_cast<double>(neighbors)));
        }
    }

    Eigen::SparseMatrix<double> L(n, n);
    L.setFromTriplets(triplets.begin(), triplets.end());

    // -------------------------------------------------------------------------
    // Resolver o sistema linear: Lz = b
    // -------------------------------------------------------------------------
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
    solver.compute(L);
    Eigen::VectorXd z = solver.solve(b);

    // -------------------------------------------------------------------------
    // Converter o vetor solução para matriz h × w
    // -------------------------------------------------------------------------
    Eigen::MatrixXd result(h, w);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            result(y, x) = z(y * w + x);

    return result;
}
