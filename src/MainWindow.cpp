#include "MainWindow.h"
#include "PaintWidget.h"
#include "ImageToGradient.h"
#include "TerrainGenerator.h"
#include "GradientField.h"
#include "ShowTerrain.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <iostream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    stack = new QStackedWidget(this);

    paintWidget = new PaintWidget(this);
    paintWidget->setMinimumSize(800, 600);
    paintWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // page1 - drawing
    page1 = new QWidget();
    QVBoxLayout* layout1 = new QVBoxLayout(page1);
    generateButton = new QPushButton(QStringLiteral("Gerar Terreno"));
    layout1->addWidget(paintWidget);
    layout1->addWidget(generateButton);

    // page2 - viewer placeholder
    page2 = new QWidget();
    QVBoxLayout* layout2 = new QVBoxLayout(page2);
    backButton = new QPushButton(QStringLiteral("\u2190 Voltar para Desenho"));
    backButton->setFixedSize(150,40);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    layout2->addLayout(buttonLayout);

    stack->addWidget(page1); // index 0
    stack->addWidget(page2); // index 1

    setCentralWidget(stack);

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateTerrain);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToDrawing);

    connect(stack, &QStackedWidget::currentChanged, [this](int index){
        if (index == 1) {
            std::cout << "DEBUG: Entering 3D view mode - painting DISABLED" << std::endl;
        } else {
            std::cout << "DEBUG: Entering drawing mode" << std::endl;
        }
    });
}

void MainWindow::onGenerateTerrain()
{
    QImage img = paintWidget->getImage();
    if (img.isNull()) return;

    QImage smallImg = img.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QImage inverted = smallImg.convertToFormat(QImage::Format_RGB32);
    for (int y=0;y<inverted.height();++y)
        for (int x=0;x<inverted.width();++x) {
            QColor c = inverted.pixelColor(x,y);
            inverted.setPixelColor(x,y, QColor(255-c.red(),255-c.green(),255-c.blue()));
        }

    int w = inverted.width();
    int h = inverted.height();

    Eigen::MatrixXd dx, dy;
    ImageToGradient::convert(inverted, dx, dy);

    TerrainGenerator generator(dx, dy);
    Eigen::MatrixXd heightmap = generator.generate();

    // normalizar terreno
    double minh = heightmap.minCoeff();
    if (minh < 0) heightmap.array() -= minh;

    // ---- AQUI! Abre janela libigl ----
    showTerrain(heightmap);

    stack->setCurrentIndex(1);
}

void MainWindow::onBackToDrawing()
{
    stack->setCurrentIndex(0);
    std::cout << "DEBUG: Returning to drawing mode" << std::endl;
}
