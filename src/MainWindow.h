#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QImage>
#include <QSlider>
#include <QLabel>

class PaintWidget;
class QPushButton;
class GradientField;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
public slots:
    void onGenerateTerrain();
    void onBackToDrawing();
    void onBrushSizeChanged(int size);
private:
    QStackedWidget* stack;
    PaintWidget* paintWidget;
    QWidget* page1;
    QWidget* page2;
    QPushButton* generateButton;
    QPushButton* clearButton;
    QPushButton* backButton;
    QSlider* brushSlider;
    QLabel* brushLabel;
    GradientField* sharedGradient = nullptr;
};
