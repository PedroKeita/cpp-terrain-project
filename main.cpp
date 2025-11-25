#include <QApplication>
#include <QMainWindow>
#include "opengl/OpenGLWidget.h"
#include "terrain/TerrainGenerator.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;

    // Gera altura
    GradientField* field = new GradientField(100, 100);
    for (int y = 0; y < 100; y++)
        for (int x = 0; x < 100; x++)
            field->setGradient(x, y, sin(x * 0.1), cos(y * 0.1));
    TerrainGenerator terrainGen(100, 100, field);
    Eigen::MatrixXd heightMap = terrainGen.generate();

    std::cout << "Altura mínima = " << heightMap.minCoeff() << "\n";
    std::cout << "Altura máxima = " << heightMap.maxCoeff() << "\n";


    // Cria OpenGLWidget único
    OpenGLWidget* ogl = new OpenGLWidget;

    // Passa heightmap
    ogl->setHeightMap(heightMap);

    // Passa campo de gradiente
    ogl->setGradientField(field);

    // Coloca na janela
    window.setCentralWidget(ogl);
    window.resize(800, 600);
    window.show();

    return a.exec();
}
