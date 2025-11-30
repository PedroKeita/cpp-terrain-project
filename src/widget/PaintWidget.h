#pragma once
#include <QWidget>
#include <QImage>
#include <QPoint>

class GradientField;

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);

    void setGradientField(GradientField* field);
    QImage getImage() const;

signals:
    void drawingChanged(const QImage &img);

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    void drawLineTo(const QPoint &);

    QImage image;
    QImage canvas;

    QPoint lastPoint;
    bool drawing = false;

    GradientField *gradientField = nullptr;
};
