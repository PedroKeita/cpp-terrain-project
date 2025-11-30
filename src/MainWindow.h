#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

#include "widget/PaintWidget.h"
#include "opengl/OpenGLWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stack;
    PaintWidget *paintWidget;
    OpenGLWidget *openGLWidget;
    QPushButton *generateButton;

    QImage currentDrawing;

private slots:
    void onGenerateTerrain();
};

#endif
