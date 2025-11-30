#include "MainWindow.h"
#include "utils/ImageToGradient.h"
#include "TerrainGenerator.h"
#include "terrain/PoissonSolver.h"  // ← ADICIONE ESTA LINHA
#include <Eigen/Dense>
#include <QVBoxLayout>
#include <QPushButton>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    stack = new QStackedWidget(this);

    paintWidget = new PaintWidget(this);
    paintWidget->setMinimumSize(800, 600);   // manipula a area do desenho
    paintWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    openGLWidget = new OpenGLWidget();

    // Tela 1 — layout com botão e PaintWidget
    QWidget *page1 = new QWidget();
    QVBoxLayout *layout1 = new QVBoxLayout(page1);

    QPushButton *generateButton = new QPushButton("Gerar Terreno");
    layout1->addWidget(paintWidget);
    layout1->addWidget(generateButton);

    stack->addWidget(page1);         // index 0
    stack->addWidget(openGLWidget);  // index 1

    setCentralWidget(stack);

    connect(generateButton, &QPushButton::clicked,
            this, &MainWindow::onGenerateTerrain);
}

void MainWindow::onGenerateTerrain()
{
    QImage img = paintWidget->getImage();
    if (img.isNull()) {
        std::cout << "DEBUG: Image is null" << std::endl;
        return;  // Não faz nada se não há imagem
    }

    // REDUZ o tamanho para algo gerenciável
    QImage smallImg = img.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int w = smallImg.width();
    int h = smallImg.height();

    std::cout << "DEBUG: Using resized image: " << w << "x" << h << std::endl;

    GradientField* sharedGradient = new GradientField(w, h);

    Eigen::MatrixXd dx, dy;
    ImageToGradient::convert(smallImg, dx, dy);

    std::cout << "DEBUG: Gradient dx - Min: " << dx.minCoeff() << " Max: " << dx.maxCoeff() << std::endl;
    std::cout << "DEBUG: Gradient dy - Min: " << dy.minCoeff() << " Max: " << dy.maxCoeff() << std::endl;

    // Popular o gradientField
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            sharedGradient->setGradient(x, y, dx(y,x), dy(y,x));
        }
    }

    // Configurar nos widgets
    paintWidget->setGradientField(sharedGradient);
    openGLWidget->setGradientField(sharedGradient);

    // Gerar terreno - AGORA USA O POISSON SOLVER REAL!
    PoissonSolver solver(*sharedGradient);
    Eigen::MatrixXd heightmap = solver.solve();

    std::cout << "DEBUG: Real terrain - Min: " << heightmap.minCoeff()
              << " Max: " << heightmap.maxCoeff() << std::endl;

    openGLWidget->setHeightMap(heightmap);
    stack->setCurrentIndex(1);
}
