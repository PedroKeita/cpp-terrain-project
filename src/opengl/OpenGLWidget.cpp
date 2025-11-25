#include "OpenGLWidget.h"
#include <GL/glu.h>
#include <cmath>
#include <QMouseEvent>
#include <QWheelEvent>
#include "terrain/GradientField.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void OpenGLWidget::setHeightMap(const Eigen::MatrixXd &heightMap)
{
    m_heightMap = heightMap;
    hasHeightMap = true;
    update();
}

void OpenGLWidget::setGradientField(GradientField* field)
{
    gradField = field;
    update();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(1, 1, 1, 1);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, double(w)/h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// --- Variáveis de câmera ---
float cameraRotX = 35.0f;
float cameraRotY = -45.0f;
float cameraDist = 3.0f;

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (!hasHeightMap || m_heightMap.size() == 0)
        return;

    int w = m_heightMap.rows();
    int h = m_heightMap.cols();

    // --- NORMALIZA ALTURAS PARA [0, 15] ---
    double minH = m_heightMap.minCoeff();
    double maxH = m_heightMap.maxCoeff();
    double scaleH = 15.0 / (maxH - minH);
    Eigen::MatrixXd normalized = (m_heightMap.array() - minH) * scaleH;

    // --- ESCALA HORIZONTAL ---
    float horizontalScale = 2.0f;

    // --- CENTRALIZA TERRENO ---
    float terrainCenterX = w * horizontalScale / 2.0f;
    float terrainCenterZ = h * horizontalScale / 2.0f;
    float terrainHeightOffset = 7.5f; // metade da altura máxima

    // --- CÂMERA ---
    glTranslatef(0, -terrainHeightOffset, -terrainCenterZ - cameraDist * w * 0.5f);
    glRotatef(cameraRotX, 1, 0, 0);
    glRotatef(cameraRotY, 0, 1, 0);
    glTranslatef(-terrainCenterX, 0, 0);

    // --- DESENHO DO TERRENO ---
    glColor3f(0.5f, 0.35f, 0.2f);

    for (int y = 0; y < h - 1; ++y) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int x = 0; x < w; ++x) {
            double h1 = normalized(x, y);
            double h2 = normalized(x, y + 1);

            glVertex3f(x * horizontalScale, h1, y * horizontalScale);
            glVertex3f(x * horizontalScale, h2, (y + 1) * horizontalScale);
        }
        glEnd();
    }
}

// --- INTERAÇÃO MOUSE ---
void OpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    lastMouse = e->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - lastMouse.x();
    int dy = e->y() - lastMouse.y();

    cameraRotY += dx * 0.5f;
    cameraRotX += dy * 0.5f;

    lastMouse = e->pos();
    update();
}

// --- ZOOM COM RODA DO MOUSE ---
void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    cameraDist -= event->angleDelta().y() / 120 * 0.5f;

    if(cameraDist < 0.5f) cameraDist = 0.5f;
    if(cameraDist > 50.0f) cameraDist = 50.0f;

    update();
}
