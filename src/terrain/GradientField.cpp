#include "GradientField.h" // Aqui pega o header de GradientField, é tipo uma interface

GradientField::GradientField(int width, int height) //Aqui é um construtor da classe
    : width_(width), height_(height), // Usa esses dois pontos para inicializar os membros e seus respectivos argumentos

    fieldX(Eigen::MatrixXd::Zero(height, width)), // esses field são objetos Eigen::MatrixXd
    fieldY(Eigen::MatrixXd::Zero(height, width)) {}

void GradientField::setGradient(int x, int y, double dx, double dy) {
    fieldX(y,x) = dx;
    fieldY(y,x) = dy;
}

Eigen::Vector2d GradientField::getGradient(int x, int y) const {
    return Eigen::Vector2d(fieldX(y,x), fieldY(y,x));
}

//OBS: Lembrar que essa classe é a implementação do (header) do GradientField
