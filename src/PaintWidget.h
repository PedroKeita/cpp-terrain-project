#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>

class PaintWidget : public QWidget {
    Q_OBJECT
public:
    explicit PaintWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(400, 300);
        brushSize = 4;
    }

    QImage getImage() const { return image; }

    void clear() {
        image.fill(Qt::white);
        update();
    }

    void setBrushSize(int size) {
        brushSize = size;
    }

    int getBrushSize() const {
        return brushSize;
    }

protected:
    void resizeEvent(QResizeEvent* e) override {
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

    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.drawImage(0, 0, image);
    }

    void mousePressEvent(QMouseEvent* e) override {
        lastPoint = e->pos();
        drawing = true;
        // Desenhar ponto inicial
        QPainter p(&image);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.drawEllipse(e->pos(), brushSize, brushSize);
        update();
    }

    void mouseMoveEvent(QMouseEvent* e) override {
        if (drawing) {
            QPainter p(&image);
            p.setRenderHint(QPainter::Antialiasing, true);
            p.setPen(QPen(Qt::black, brushSize * 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(lastPoint, e->pos());
            lastPoint = e->pos();
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent*) override {
        drawing = false;
    }

private:
    QImage image;
    QPoint lastPoint;
    bool drawing = false;
    int brushSize;
};
