#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>

class PaintWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);
    const QImage &getImage() const { return image; }

signals:
    void drawingChanged(const QImage &img); // Emite quando desenha

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QImage image;
    QPoint lastPoint;
    bool drawing = false;

    void drawLineTo(const QPoint &endPoint);
};

#endif
