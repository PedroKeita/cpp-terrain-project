#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QImage>

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
private:
    QStackedWidget* stack;
    PaintWidget* paintWidget;
    QWidget* page1;
    QWidget* page2;
    QPushButton* generateButton;
    QPushButton* backButton;
    GradientField* sharedGradient = nullptr;
};
