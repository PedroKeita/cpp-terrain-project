#include "MainWindow.h"
#include "PaintWidget.h"
#include "ShowTerrain.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QSlider>
#include <QFrame>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Estilo global escuro e moderno
    QString style = R"(
        * {
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QMainWindow, QWidget {
            background-color: #121212;
            color: #e0e0e0;
        }
        QPushButton {
            background-color: #2d2d2d;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 12px 24px;
            font-size: 14px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #3d3d3d;
        }
        QPushButton:pressed {
            background-color: #1d1d1d;
        }
        QPushButton#generateBtn {
            background-color: #1a73e8;
            font-size: 16px;
            padding: 16px 32px;
        }
        QPushButton#generateBtn:hover {
            background-color: #2b7de9;
        }
        QPushButton#clearBtn {
            background-color: #c62828;
        }
        QPushButton#clearBtn:hover {
            background-color: #d32f2f;
        }
        QPushButton#backBtn {
            background-color: #424242;
        }
        QPushButton#backBtn:hover {
            background-color: #616161;
        }
        QLabel {
            color: #b0b0b0;
            font-size: 13px;
        }
        QLabel#title {
            color: #ffffff;
            font-size: 24px;
            font-weight: bold;
        }
        QLabel#subtitle {
            color: #888888;
            font-size: 14px;
        }
        QSlider::groove:horizontal {
            border: none;
            height: 6px;
            background: #3d3d3d;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #1a73e8;
            width: 18px;
            height: 18px;
            margin: -6px 0;
            border-radius: 9px;
        }
        QSlider::handle:horizontal:hover {
            background: #2b7de9;
        }
        QFrame#toolbar {
            background-color: #1e1e1e;
            border-radius: 12px;
            padding: 10px;
        }
        QFrame#canvas {
            background-color: #ffffff;
            border-radius: 8px;
        }
    )";
    setStyleSheet(style);

    stack = new QStackedWidget(this);

    // ========== PAGINA 1 - DESENHO ==========
    page1 = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(page1);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Header
    QLabel* titleLabel = new QLabel("Gerador de Terreno 3D");
    titleLabel->setObjectName("title");

    QLabel* subtitleLabel = new QLabel("Desenhe o perfil do terreno - a linha define o topo da montanha");
    subtitleLabel->setObjectName("subtitle");

    // Toolbar
    QFrame* toolbar = new QFrame();
    toolbar->setObjectName("toolbar");
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(15, 10, 15, 10);
    toolbarLayout->setSpacing(20);

    // Brush size
    QLabel* brushIcon = new QLabel("Pincel:");
    brushSlider = new QSlider(Qt::Horizontal);
    brushSlider->setRange(1, 20);
    brushSlider->setValue(4);
    brushSlider->setFixedWidth(150);

    brushLabel = new QLabel("4px");
    brushLabel->setFixedWidth(40);

    // Botao limpar
    clearButton = new QPushButton("Limpar");
    clearButton->setObjectName("clearBtn");
    clearButton->setFixedWidth(100);

    toolbarLayout->addWidget(brushIcon);
    toolbarLayout->addWidget(brushSlider);
    toolbarLayout->addWidget(brushLabel);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(clearButton);

    // Canvas de desenho
    QFrame* canvasFrame = new QFrame();
    canvasFrame->setObjectName("canvas");
    QVBoxLayout* canvasLayout = new QVBoxLayout(canvasFrame);
    canvasLayout->setContentsMargins(0, 0, 0, 0);

    paintWidget = new PaintWidget(this);
    paintWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvasLayout->addWidget(paintWidget);

    // Botao gerar
    generateButton = new QPushButton("Gerar Terreno 3D");
    generateButton->setObjectName("generateBtn");
    generateButton->setCursor(Qt::PointingHandCursor);

    // Layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(canvasFrame, 1);
    mainLayout->addWidget(generateButton, 0, Qt::AlignCenter);

    // ========== PAGINA 2 - VISUALIZADOR ==========
    page2 = new QWidget();
    QVBoxLayout* layout2 = new QVBoxLayout(page2);
    layout2->setContentsMargins(30, 30, 30, 30);
    layout2->setSpacing(20);

    // Header pagina 2
    backButton = new QPushButton("<-  Voltar ao Desenho");
    backButton->setObjectName("backBtn");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setFixedWidth(200);

    QLabel* viewerTitle = new QLabel("Visualizador 3D");
    viewerTitle->setObjectName("title");

    QLabel* viewerInfo = new QLabel("O terreno foi gerado em uma janela separada.\n\nControles:\n- Arrastar mouse: Rotacionar\n- Scroll: Zoom\n- Shift + Arrastar: Mover\n- L: Wireframe\n- O: Ortografico/Perspectiva\n- ESC: Fechar");
    viewerInfo->setStyleSheet("color: #888888; font-size: 14px; line-height: 1.6;");
    viewerInfo->setAlignment(Qt::AlignCenter);

    layout2->addWidget(backButton, 0, Qt::AlignLeft);
    layout2->addStretch();
    layout2->addWidget(viewerTitle, 0, Qt::AlignCenter);
    layout2->addWidget(viewerInfo, 0, Qt::AlignCenter);
    layout2->addStretch();

    // Stack
    stack->addWidget(page1);
    stack->addWidget(page2);
    setCentralWidget(stack);

    // Conexoes
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateTerrain);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToDrawing);
    connect(clearButton, &QPushButton::clicked, [this]() {
        paintWidget->clear();
    });
    connect(brushSlider, &QSlider::valueChanged, this, &MainWindow::onBrushSizeChanged);

    // Tamanho inicial
    resize(1000, 750);
}

void MainWindow::onBrushSizeChanged(int size)
{
    paintWidget->setBrushSize(size);
    brushLabel->setText(QString::number(size) + "px");
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
    Eigen::MatrixXd heightmap(h, w);
    heightmap.setZero();

    for (int x = 0; x < w; ++x) {
        // Procurar o primeiro pixel preto de cima para baixo
        int lineY = -1;
        for (int y = 0; y < h; ++y) {
            QColor c = smallImg.pixelColor(x, y);
            if (c.red() < 200 || c.green() < 200 || c.blue() < 200) {
                lineY = y;
                break;
            }
        }

        // Criar montanha abaixo da linha
        for (int y = 0; y < h; ++y) {
            if (lineY < 0 || y <= lineY) {
                heightmap(h - 1 - y, x) = 0;
            } else {
                double distFromLine = (double)(y - lineY) / (double)(h - lineY);
                double altura = (1.0 - distFromLine) * 80.0;
                heightmap(h - 1 - y, x) = altura;
            }
        }
    }

    // Suavizar
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

    stack->setCurrentIndex(1);
    showTerrain(heightmap);
}

void MainWindow::onBackToDrawing()
{
    stack->setCurrentIndex(0);
}
