/**
 * @file MainWindow.cpp
 * @brief Implementação da janela principal da aplicação de geração de terreno 3D.
 *
 * Esta classe gerencia:
 * - Interface gráfica do usuário
 * - Página de desenho
 * - Página de visualização 3D
 * - Conversão do desenho em mapa de alturas
 * - Aplicação de ruído, suavização e erosão
 * - Envio do heightmap para o renderizador 3D
 */

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
#include "PerlinNoise.h"
#include "ImageToGradient.h"

/**
 * @brief Constrói a janela principal e inicializa toda a interface gráfica.
 * @param parent Widget pai da janela principal.
 */
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

/**
 * @brief Atualiza o tamanho do pincel ao mover o slider.
 * @param size Tamanho do pincel em pixels.
 *
 * Slot conectado ao sinal QSlider::valueChanged.
 */
void MainWindow::onBrushSizeChanged(int size)
{
    paintWidget->setBrushSize(size);
    brushLabel->setText(QString::number(size) + "px");
}

/**
 * @brief Processa o desenho e gera o terreno 3D.
 *
 * Passos executados:
 * - Captura o desenho do usuário
 * - Redimensiona imagem
 * - Extrai gradiente com Sobel
 * - Calcula distância até pontos desenhados
 * - Forma base da montanha
 * - Aplica ruído Perlin multiescala
 * - Suaviza e aplica erosão leve
 * - Envia heightmap ao renderizador 3D
 *
 * Slot conectado ao botão "Gerar Terreno 3D".
 */
void MainWindow::onGenerateTerrain()
{
    QImage img = paintWidget->getImage();
    if (img.isNull()) return;

    int terrainSize = 256;
    QImage smallImg = img.scaled(terrainSize, terrainSize,
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);

    int w = smallImg.width();
    int h = smallImg.height();

    Eigen::MatrixXd heightmap(h, w);
    heightmap.setZero();

    // ==== SOBEL ===
    Eigen::MatrixXd Gx, Gy;
    ImageToGradient::convert(smallImg, Gx, Gy);

    // Calcular magnitude do gradiente
    Eigen::MatrixXd gradientMagnitude(h, w);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // Fórmula: |∇I| = √(Gx² + Gy²)
            gradientMagnitude(y, x) = sqrt(Gx(y,x)*Gx(y,x) + Gy(y,x)*Gy(y,x));
        }
    }

    std::vector<std::tuple<int, int, double>> blackPoints;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            QColor c = smallImg.pixelColor(x, y);
            double intensity = 1.0 - (c.redF() * 0.299 + c.greenF() * 0.587 + c.blueF() * 0.114);

            if (intensity > 0.1) {
                blackPoints.push_back({x, y, std::min(1.0, intensity * 2.0)});
                heightmap(y, x) = intensity * 100.0;
            }
        }
    }

    if (blackPoints.empty()) {
        double maxGrad = gradientMagnitude.maxCoeff();
        if (maxGrad > 0) {
            heightmap = gradientMagnitude * (80.0 / maxGrad);
        }

        stack->setCurrentIndex(1);
        showTerrain(heightmap);
        return;
    }


    Eigen::MatrixXd minDistanceToBlack(h, w);
    minDistanceToBlack.setConstant(std::numeric_limits<double>::max());

    for (const auto& blackPoint : blackPoints) {
        int px = std::get<0>(blackPoint);
        int py = std::get<1>(blackPoint);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                double dx = x - px;
                double dy = y - py;
                double distance = sqrt(dx*dx + dy*dy);

                if (distance < minDistanceToBlack(y, x)) {
                    minDistanceToBlack(y, x) = distance;
                }
            }
        }
    }

    double maxDistance = 0;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (minDistanceToBlack(y, x) > maxDistance) {
                maxDistance = minDistanceToBlack(y, x);
            }
        }
    }


    double falloffEnd = maxDistance * 0.8;
    double baseTerrainHeight = 5.0;


    double maxGrad = gradientMagnitude.maxCoeff();
    if (maxGrad > 0) {
        gradientMagnitude = gradientMagnitude / maxGrad;
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double distance = minDistanceToBlack(y, x);

            if (distance <= falloffEnd) {
                double normalizedDist = (falloffEnd > 0) ? distance / falloffEnd : 0;


                double decayFactor;
                if (distance == 0) {
                    decayFactor = 1.0;
                } else {
                    double t = 1.0 - normalizedDist;
                    decayFactor = t * t * (3.0 - 2.0 * t);
                    decayFactor = pow(decayFactor, 0.7);
                }

                double baseHeight = 80.0 * decayFactor;


                double gradientInfluence = gradientMagnitude(y, x) * 20.0; // Escalar
                double combinedHeight = baseHeight + gradientInfluence;

                // Se for ponto preto original, manter alta
                for (const auto& blackPoint : blackPoints) {
                    int px = std::get<0>(blackPoint);
                    int py = std::get<1>(blackPoint);
                    if (x == px && y == py) {
                        double intensity = std::get<2>(blackPoint);
                        combinedHeight = std::max(combinedHeight, intensity * 100.0);
                        break;
                    }
                }

                heightmap(y, x) = combinedHeight;
            } else {
                heightmap(y, x) = baseTerrainHeight;
            }
        }
    }

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            // Ângulo do gradiente (direção da maior variação)
            double angle = atan2(Gy(y, x), Gx(y, x));

            // Aplicar leve inclinação baseada no ângulo
            double slopeEffect = sin(angle) * 2.0; // Pequeno efeito
            heightmap(y, x) += slopeEffect;
        }
    }



    double transitionWidth = maxDistance * 0.2;
    double transitionStart = falloffEnd - transitionWidth;
    if (transitionStart < 0) transitionStart = 0;

    PerlinNoise noiseGen(12345);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double distance = minDistanceToBlack(y, x);

            if (distance > transitionStart && distance <= falloffEnd) {
                double blendFactor = (distance - transitionStart) / transitionWidth;
                double mountainHeight = heightmap(y, x);
                double baseNoise = noiseGen.octaveNoise(x * 0.01, y * 0.01, 1, 0.5);
                double terrainHeight = baseTerrainHeight + baseNoise * 3.0;
                heightmap(y, x) = mountainHeight * (1.0 - blendFactor) +
                                 terrainHeight * blendFactor;
            } else if (distance > falloffEnd) {
                double baseNoise = noiseGen.octaveNoise(x * 0.01, y * 0.01, 1, 0.5);
                heightmap(y, x) = baseTerrainHeight + baseNoise * 3.0;
            }
        }
    }

    // Ruído Perlin
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double currentHeight = heightmap(y, x);
            double normalizedHeight = currentHeight / 80.0;

            if (normalizedHeight > 0.1) {
                double noiseLow = noiseGen.octaveNoise(x * 0.03, y * 0.03, 1, 0.5);
                double noiseMid = noiseGen.octaveNoise(x * 0.08, y * 0.08, 2, 0.7);
                double noiseHigh = noiseGen.octaveNoise(x * 0.2, y * 0.2, 3, 0.8);

                double totalNoise = (noiseLow * 0.2 + noiseMid * 0.5 + noiseHigh * 0.3) *
                                   normalizedHeight * 25.0;

                heightmap(y, x) += totalNoise;
            }
        }
    }

    // Suavização
    for (int iter = 0; iter < 3; ++iter) {
        Eigen::MatrixXd smoothed = heightmap;

        for (int y = 1; y < h - 1; ++y) {
            for (int x = 1; x < w - 1; ++x) {
                double current = heightmap(y, x);
                double neighbors[4] = {
                    heightmap(y-1, x), heightmap(y+1, x),
                    heightmap(y, x-1), heightmap(y, x+1)
                };
                double avgNeighbors = (neighbors[0] + neighbors[1] +
                                      neighbors[2] + neighbors[3]) / 4.0;
                double gradient = 0;
                for (int i = 0; i < 4; i++) {
                    gradient += std::abs(current - neighbors[i]);
                }
                gradient /= 4.0;
                double smoothFactor = std::min(1.0, gradient / 30.0);
                smoothed(y, x) = current * (1.0 - smoothFactor) +
                                avgNeighbors * smoothFactor;
            }
        }
        heightmap = smoothed;
    }

    applySoftErosion(heightmap, 2);
    stack->setCurrentIndex(1);
    showTerrain(heightmap);
}

/**
 * @brief Retorna da página de visualização 3D para a página de desenho.
 *
 * Slot conectado ao botão de voltar.
 */
void MainWindow::onBackToDrawing()
{
    stack->setCurrentIndex(0);
}

/**
 * @brief Aplica erosão suave no heightmap para reduzir ângulos bruscos.
 *
 * A erosão funciona reduzindo alturas onde existe uma diferença
 * significativa em relação a vizinhos mais baixos, simulando
 * desmoronamento de areia/solo.
 *
 * @param heightmap Matriz de alturas a ser modificada.
 * @param iterations Quantidade de iterações de erosão aplicadas.
 */
void MainWindow::applySoftErosion(Eigen::MatrixXd& heightmap, int iterations) {
    int h = heightmap.rows();
    int w = heightmap.cols();

    for (int iter = 0; iter < iterations; ++iter) {
        Eigen::MatrixXd eroded = heightmap;

        for (int y = 1; y < h - 1; ++y) {
            for (int x = 1; x < w - 1; ++x) {
                double current = heightmap(y, x);

                // Encontrar o vizinho mais baixo
                double minNeighbor = current;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        double neighbor = heightmap(y + dy, x + dx);
                        if (neighbor < minNeighbor) {
                            minNeighbor = neighbor;
                        }
                    }
                }

                // Suavização com erosão
                double diff = current - minNeighbor;
                if (diff > 10.0) { // Limiar para erosão
                    // Transferir um pouco de material para baixo
                    double transfer = diff * 0.05;
                    eroded(y, x) -= transfer;

                    // Distribuir para os vizinhos mais baixos
                    int lowCount = 0;
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if (dx == 0 && dy == 0) continue;
                            if (heightmap(y + dy, x + dx) < current - 1.0) {
                                lowCount++;
                            }
                        }
                    }

                    if (lowCount > 0) {
                        double perNeighbor = transfer / lowCount;
                        for (int dy = -1; dy <= 1; dy++) {
                            for (int dx = -1; dx <= 1; dx++) {
                                if (dx == 0 && dy == 0) continue;
                                if (heightmap(y + dy, x + dx) < current - 1.0) {
                                    eroded(y + dy, x + dx) += perNeighbor;
                                }
                            }
                        }
                    }
                }
            }
        }

        heightmap = eroded;
    }
}

