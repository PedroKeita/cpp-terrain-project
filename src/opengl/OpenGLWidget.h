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
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPoint screenToTerrain(const QPoint& p);
    void applyBrushStroke(const QPoint& tPos, const QPoint& tPrev);

    Eigen::MatrixXd m_heightMap;
    GradientField* gradField = nullptr;

    bool hasHeightMap = false;
    bool painting = false;     // botão esquerdo
    bool rotating = false;     // botão direito

    QPoint lastMouse;

    float brushStrength = 1.0f;
    float brushRadius = 3.0f;

    // --- Câmera ---
    float cameraRotX = 45.0f;
    float cameraRotY = -45.0f;
    float cameraDist = 50.0f;
    float heightOffset = 7.5f;
};

#endif
