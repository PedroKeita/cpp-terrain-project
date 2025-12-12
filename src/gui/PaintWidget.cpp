/**
 * @file PaintWidget.cpp
 * @brief Implementation of PaintWidget class for interactive terrain profile drawing.
 */

#include "../../include/terrain-generator/PaintWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPen>

/**
 * @brief Constructor for PaintWidget class
 * 
 * Initializes the drawing canvas with default white background
 * and sets minimum size to 400x300 pixels.
 */
PaintWidget::PaintWidget(QWidget* parent) 
    : QWidget(parent)
    , brushSize(4)
{
    setMinimumSize(400, 300);
}

/**
 * @brief Get the current drawing as a QImage
 * 
 * @return Current drawing image
 */
QImage PaintWidget::getImage() const {
    return image;
}

/**
 * @brief Clear the drawing canvas
 * 
 * Resets canvas to white background and triggers repaint.
 */
void PaintWidget::clear() {
    image.fill(Qt::white);
    update();
}

/**
 * @brief Set brush size for drawing
 * 
 * @param size New brush diameter in pixels
 */
void PaintWidget::setBrushSize(int size) {
    brushSize = size;
}

/**
 * @brief Get current brush size
 * 
 * @return Current brush diameter in pixels
 */
int PaintWidget::getBrushSize() const {
    return brushSize;
}

/**
 * @brief Handle widget resize events
 * 
 * Maintains drawing when widget is resized by scaling
 * existing content to new size.
 */
void PaintWidget::resizeEvent(QResizeEvent* e) {
    if (image.isNull() || image.size() != e->size()) {
        QImage newImage(e->size(), QImage::Format_RGB32);
        newImage.fill(Qt::white);
        if (!image.isNull()) {
            QPainter p(&newImage);
            p.drawImage(0, 0, image.scaled(e->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        image = newImage;
    }
    QWidget::resizeEvent(e);
}

/**
 * @brief Handle paint events
 * 
 * Renders current drawing image onto widget surface.
 */
void PaintWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.drawImage(0, 0, image);
}

/**
 * @brief Handle mouse press events
 * 
 * Starts new drawing stroke and draws initial circle.
 */
void PaintWidget::mousePressEvent(QMouseEvent* e) {
    lastPoint = e->pos();
    drawing = true;
    
    // Draw starting point
    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawEllipse(e->pos(), brushSize, brushSize);
    update();
}

/**
 * @brief Handle mouse move events
 * 
 * Continues current drawing stroke with line.
 */
void PaintWidget::mouseMoveEvent(QMouseEvent* e) {
    if (drawing) {
        QPainter p(&image);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setPen(QPen(Qt::black, brushSize * 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawLine(lastPoint, e->pos());
        lastPoint = e->pos();
        update();
    }
}

/**
 * @brief Handle mouse release events
 * 
 * Ends current drawing stroke.
 */
void PaintWidget::mouseReleaseEvent(QMouseEvent*) {
    drawing = false;
}
