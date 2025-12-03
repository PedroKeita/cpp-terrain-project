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
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Tema escuro
    QString darkStyle = R"(
        QMainWindow, QWidget {
            background-color: #1a1a1a;
            color: #ffffff;
        }
        QPushButton {
            background-color: #3a3a3a;
            color: #ffffff;
            border: 1px solid #555555;
            border-radius: 5px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
            border: 1px solid #888888;
        }
        QPushButton:pressed {
            background-color: #2a2a2a;
        }
        QLabel {
            color: #ffffff;
            font-size: 12px;
        }
    )";
    setStyleSheet(darkStyle);

    stack = new QStackedWidget(this);

    paintWidget = new PaintWidget(this);
    paintWidget->setMinimumSize(800, 600);
    paintWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // page1 - drawing
    page1 = new QWidget();
    QVBoxLayout* layout1 = new QVBoxLayout(page1);
    layout1->setContentsMargins(20, 20, 20, 20);
    layout1->setSpacing(15);

    QLabel* titleLabel = new QLabel("Desenhe o terreno abaixo:");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ffffff;");

    generateButton = new QPushButton("Gerar Terreno 3D");
    generateButton->setFixedHeight(50);
    generateButton->setStyleSheet(R"(
        QPushButton {
            background-color: #2e7d32;
            color: #ffffff;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #388e3c;
        }
    )");

    QPushButton* clearButton = new QPushButton("Limpar Desenho");
    clearButton->setFixedHeight(40);

    QHBoxLayout* buttonRow = new QHBoxLayout();
    buttonRow->addWidget(generateButton);
    buttonRow->addWidget(clearButton);

    layout1->addWidget(titleLabel);
    layout1->addWidget(paintWidget);
    layout1->addLayout(buttonRow);

    // page2 - placeholder apos gerar
    page2 = new QWidget();
    QVBoxLayout* layout2 = new QVBoxLayout(page2);
    layout2->setContentsMargins(20, 20, 20, 20);

    backButton = new QPushButton("<- Voltar para Desenho");
    backButton->setFixedSize(200, 45);

    QLabel* infoLabel = new QLabel("Visualizador 3D aberto em outra janela.\nFeche a janela 3D para voltar.");
    infoLabel->setStyleSheet("font-size: 14px; color: #aaaaaa;");
    infoLabel->setAlignment(Qt::AlignCenter);

    layout2->addWidget(backButton, 0, Qt::AlignLeft);
    layout2->addStretch();
    layout2->addWidget(infoLabel, 0, Qt::AlignCenter);
    layout2->addStretch();

    stack->addWidget(page1);
    stack->addWidget(page2);

    setCentralWidget(stack);

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateTerrain);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToDrawing);
    connect(clearButton, &QPushButton::clicked, [this]() {
        paintWidget->clear();
    });
}

void MainWindow::onGenerateTerrain()
{
    QImage img = paintWidget->getImage();
    if (img.isNull()) return;

    // Redimensionar para tamanho do terreno
    int terrainSize = 256;
    QImage smallImg = img.scaled(terrainSize, terrainSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    int w = smallImg.width();
    int h = smallImg.height();

    // Criar heightmap baseado no desenho
    // Para cada coluna X, encontrar a linha desenhada
    // A altura em Y vai de 0 (embaixo da linha) ate maximo (na linha)
    Eigen::MatrixXd heightmap(h, w);
    heightmap.setZero();

    for (int x = 0; x < w; ++x) {
        // Procurar o primeiro pixel preto (desenhado) de cima para baixo
        int lineY = -1; // se nao encontrar, sem montanha
        for (int y = 0; y < h; ++y) {
            QColor c = smallImg.pixelColor(x, y);
            // Pixel escuro = linha desenhada
            if (c.red() < 200 || c.green() < 200 || c.blue() < 200) {
                lineY = y;
                break;
            }
        }

        // Se encontrou linha, criar montanha abaixo dela
        // Pico na linha, decresce ate o fim da tela
        for (int y = 0; y < h; ++y) {
            if (lineY < 0 || y <= lineY) {
                // Acima da linha ou sem linha = plano (altura 0)
                heightmap(h - 1 - y, x) = 0;
            } else {
                // Abaixo da linha = montanha
                // Pico logo abaixo da linha, decresce conforme desce
                double distFromLine = (double)(y - lineY) / (double)(h - lineY);
                double altura = (1.0 - distFromLine) * 80.0;
                heightmap(h - 1 - y, x) = altura;
            }
        }
    }

    // Suavizar o heightmap
    Eigen::MatrixXd smoothed = heightmap;
    for (int iter = 0; iter < 3; ++iter) {
        for (int y = 1; y < h - 1; ++y) {
            for (int x = 1; x < w - 1; ++x) {
                smoothed(y, x) = (heightmap(y, x) * 2 +
                                  heightmap(y-1, x) + heightmap(y+1, x) +
                                  heightmap(y, x-1) + heightmap(y, x+1)) / 6.0;
            }
        }
        heightmap = smoothed;
    }

    showTerrain(heightmap);

    stack->setCurrentIndex(1);
}

void MainWindow::onBackToDrawing()
{
    stack->setCurrentIndex(0);
}
