#pragma once
#include <Eigen/Dense> //Biblioteca Eigen, para operações matemáticas

class GradientField {
public: // Aqui vai definir os métodos que são acessiveis fora da classe

    // Construtor padrão com largura e altura opcionais OBS: Alterar dps
    GradientField(int width = 0, int height = 0);

    // Define o gradiente no ponto (x, y)
    void setGradient(int x, int y, double dx, double dy);

    // retorna o gradiente como um vetor bidimensional da biblio Eigen
    Eigen::Vector2d getGradient(int x, int y) const;

    //Getters
    int width()  const { return width_; }
    int height() const { return height_; }

private: // métodos privados
    int width_, height_;
    Eigen::MatrixXd fieldX, fieldY;

};


