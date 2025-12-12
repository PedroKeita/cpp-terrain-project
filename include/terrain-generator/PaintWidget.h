/**
* @file PaintWidget.h
 * @brief Canvas for drawing 2D terrain profiles
 */
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>

/**
 * @class PaintWidget
 * @brief Interactive drawing canvas for terrain profile input
 *
 * Users draw black lines representing terrain cross-sections,
 * which are converted to 3D heightmaps.
 */
class PaintWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaintWidget(QWidget* parent = nullptr);

    QImage getImage() const;      ///< Retrieve current drawing
    void clear();                 ///< Reset canvas to white
    void setBrushSize(int size);  ///< Set brush diameter in pixels
    int getBrushSize() const;     ///< Get current brush size

protected:
    void resizeEvent(QResizeEvent* e) override;
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

private:
    QImage image;           ///< Internal drawing buffer
    QPoint lastPoint;       ///< Last mouse position for line drawing
    bool drawing = false;   ///< Currently drawing stroke
    int brushSize = 4;      ///< Current brush diameter (pixels)

    // Non-copyable
    PaintWidget(const PaintWidget&) = delete;
    PaintWidget& operator=(const PaintWidget&) = delete;
};

#endif