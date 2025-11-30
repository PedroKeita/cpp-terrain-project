#include "OpenGLWidget.h"
#include <GL/glu.h>
#include <cmath>
#include "terrain/TerrainGenerator.h"
#include <iostream>


OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    gradField(nullptr),
    hasHeightMap(false),
    painting(false),
    rotating(false),
    cameraDist(50.0f),      // Aumente a distância inicial
    cameraRotX(45.0f),
    cameraRotY(0.0f),
    brushRadius(5)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

// ------- SETTERS -------
void OpenGLWidget::setHeightMap(const Eigen::MatrixXd &heightMap)
{
    m_heightMap = heightMap;
    hasHeightMap = true;
    update();
}

void OpenGLWidget::setGradientField(GradientField* field)
{
    std::cout << "DEBUG: Setting gradient field: " << field << std::endl;

    gradField = field;
    update();
}


// ------- OPENGL INIT -------
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
    gluPerspective(45.0, double(w)/h, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}


// ------- DESENHO DO TERRENO -------
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (!hasHeightMap || m_heightMap.size() == 0) {
        // Debug: desenha um quadrado colorido simples se não há heightmap
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();

        std::cout << "DEBUG: No heightmap available, drawing test quad" << std::endl;
        return;
    }

    int w = m_heightMap.rows();
    int h = m_heightMap.cols();

    // Debug: imprima informações da heightmap
    std::cout << "DEBUG: HeightMap " << w << "x" << h
              << " Min: " << m_heightMap.minCoeff()
              << " Max: " << m_heightMap.maxCoeff() << std::endl;

    // Normalização de altura
    double minH = m_heightMap.minCoeff();
    double maxH = m_heightMap.maxCoeff();
    double scaleH = (maxH - minH > 0) ? 15.0 / (maxH - minH) : 1;
    Eigen::MatrixXd normalized = (m_heightMap.array() - minH) * scaleH;

    std::cout << "DEBUG: Normalized Min: " << normalized.minCoeff()
              << " Max: " << normalized.maxCoeff() << std::endl;

    float horizontalScale = 0.1f;
    float cx = w * horizontalScale / 2.0f;
    float cz = h * horizontalScale / 2.0f;
    float heightOffset = 7.5f;

    std::cout << "DEBUG: Camera - dist: " << cameraDist
              << " rotX: " << cameraRotX << " rotY: " << cameraRotY << std::endl;

    // --- Aplicar câmera ---
    glTranslatef(0, 0, -cameraDist);
    glRotatef(cameraRotX, 1, 0, 0);
    glRotatef(cameraRotY, 0, 1, 0);
    glTranslatef(-cx, -heightOffset, -cz);

    // --- Desenho do terreno ---
    glColor3f(0.3f, 0.6f, 0.2f); // Verde

    int trianglesDrawn = 0;
    for (int y = 0; y < h - 1; ++y) {
        for (int x = 0; x < w - 1; ++x) {
            glBegin(GL_TRIANGLES);

            // Primeiro triângulo
            glVertex3f(x * horizontalScale, normalized(x, y), y * horizontalScale);
            glVertex3f((x+1) * horizontalScale, normalized(x+1, y), y * horizontalScale);
            glVertex3f(x * horizontalScale, normalized(x, y+1), (y+1) * horizontalScale);

            // Segundo triângulo
            glVertex3f((x+1) * horizontalScale, normalized(x+1, y), y * horizontalScale);
            glVertex3f((x+1) * horizontalScale, normalized(x+1, y+1), (y+1) * horizontalScale);
            glVertex3f(x * horizontalScale, normalized(x, y+1), (y+1) * horizontalScale);

            glEnd();
            trianglesDrawn += 2;
        }
    }

    std::cout << "DEBUG: Triangles drawn: " << trianglesDrawn << std::endl << std::endl;
}


// ------- MOUSE EVENTS -------
void OpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    lastMouse = e->pos();

    if (e->button() == Qt::LeftButton) {
        painting = true;
    }
    if (e->button() == Qt::RightButton) {
        rotating = true;
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        painting = false;

    if (e->button() == Qt::RightButton)
        rotating = false;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint current = e->pos();

    // --- ROTACIONAR CÂMERA (botão direito) ---
    if (rotating)
    {
        QPoint delta = current - lastMouse;

        cameraRotY += delta.x() * 0.3f;
        cameraRotX += delta.y() * 0.3f;

        if (cameraRotX >  89) cameraRotX = 89;
        if (cameraRotX < -89) cameraRotX = -89;

        lastMouse = current;
        update();
        return;
    }

    // --- PINTAR TERRENO (botão esquerdo) ---
    if (painting && gradField)  // ← ADICIONE VERIFICAÇÃO AQUI
    {
        QPoint tNow  = screenToTerrain(current);
        QPoint tPrev = screenToTerrain(lastMouse);

        applyBrushStroke(tNow, tPrev);

        // Recalcular terreno
        TerrainGenerator generator(
            gradField->width(),
            gradField->height(),
            gradField
            );

        auto newHeight = generator.generate();
        setHeightMap(newHeight);
    }

    lastMouse = current;
    update();
}


// ------- ZOOM -------
void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    cameraDist -= event->angleDelta().y() / 120.0f;

    if (cameraDist < 1.0f)  cameraDist = 1.0f;
    if (cameraDist > 200.0f) cameraDist = 200.0f;

    update();
}


// ------- Conversão tela -> terreno -------
QPoint OpenGLWidget::screenToTerrain(const QPoint& p)
{
    float fx = float(p.x()) / float(width());
    float fy = 1.0f - float(p.y()) / float(height()); // Inverter Y

    int tx = int(fx * gradField->width());
    int ty = int(fy * gradField->height());

    tx = std::clamp(tx, 0, gradField->width()  - 1);
    ty = std::clamp(ty, 0, gradField->height() - 1);

    return QPoint(tx, ty);
}


// ------- Aplicar gradiente com pincel -------
void OpenGLWidget::applyBrushStroke(const QPoint& tPos, const QPoint& tPrev)
{
    if (!gradField) return;

    static int strokeCounter = 0;
    strokeCounter++;

    if (strokeCounter % 10 != 0) {
        return;
    }

    // Calcular direção e intensidade baseada no movimento
    double intensity = 0.5; // Intensidade fixa ou variável
    double dx = (tPos.x() - tPrev.x()) * 0.01;
    double dy = (tPos.y() - tPrev.y()) * 0.01;

    int cx = tPos.x();
    int cy = tPos.y();

    for (int y = -brushRadius; y <= brushRadius; y++) {
        for (int x = -brushRadius; x <= brushRadius; x++) {
            if (x*x + y*y > brushRadius * brushRadius)
                continue;

            int px = cx + x;
            int py = cy + y;

            if (px < 0 || py < 0 || px >= gradField->width() || py >= gradField->height())
                continue;

            // Aplicar gradiente baseado na direção do movimento
            gradField->setGradient(px, py, dx, dy);
        }
    }
}
