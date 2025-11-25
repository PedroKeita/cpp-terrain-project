#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <Eigen/Dense>
#include "terrain/GradientField.h"
#include <QMouseEvent>
#include <QWheelEvent>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    void setHeightMap(const Eigen::MatrixXd &heightMap);
    void setGradientField(GradientField* field);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Eigen::MatrixXd m_heightMap;
    GradientField* gradField = nullptr;
    bool hasHeightMap = false;

    QPoint lastMouse;

    // Câmera
    float cameraRotX = 35.0f;
    float cameraRotY = -45.0f;
    float cameraDist = 3.5f;
};

#endif
