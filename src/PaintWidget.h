#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>

class PaintWidget : public QWidget {
    Q_OBJECT
public:
    explicit PaintWidget(QWidget* parent = nullptr) : QWidget(parent) {
        image = QImage(800, 600, QImage::Format_RGB32);
        image.fill(Qt::white);
        setMouseTracking(true);
    }

    QImage getImage() const { return image; }

    void clear() {
        image.fill(Qt::white);
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.drawImage(0, 0, image.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    void mousePressEvent(QMouseEvent* e) override {
        lastPoint = e->pos();
        drawing = true;
    }

    void mouseMoveEvent(QMouseEvent* e) override {
        if (drawing) {
            QPainter p(&image);
            p.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QPoint scaled(e->pos().x() * image.width() / width(),
                          e->pos().y() * image.height() / height());
            QPoint lastScaled(lastPoint.x() * image.width() / width(),
                              lastPoint.y() * image.height() / height());
            p.drawLine(lastScaled, scaled);
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
};
