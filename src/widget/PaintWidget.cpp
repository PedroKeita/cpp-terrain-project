#include "PaintWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include "terrain/GradientField.h"

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent),
    image(512, 512, QImage::Format_RGB32)
{
    image.fill(Qt::white);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Importante para não perder eventos após trocar de tela
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    canvas = QImage(512, 512, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);
}

void PaintWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        lastPoint = e->pos();
        drawing = true;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->pos();

    if (drawing) {
        int dx = p.x() - lastPoint.x();
        int dy = p.y() - lastPoint.y();

        int x = lastPoint.x();
        int y = lastPoint.y();

        // Verificação de bounds
        if (gradientField &&
            x >= 0 && x < gradientField->width() &&
            y >= 0 && y < gradientField->height())
        {
            gradientField->setGradient(x, y, dx, dy);
        }

        QPainter painter(&image);
        painter.setPen(QPen(Qt::black, 4));
        painter.drawLine(lastPoint, p);

        update();
        emit drawingChanged(image);
    }

    lastPoint = p;
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && drawing) {
        drawLineTo(e->pos());
        drawing = false;
    }
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, image);
}

void PaintWidget::drawLineTo(const QPoint &endPoint)
{
    QPainter p(&image);
    p.setPen(QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.drawLine(lastPoint, endPoint);

    lastPoint = endPoint;

    update();
    emit drawingChanged(image);
}

void PaintWidget::resizeEvent(QResizeEvent *event)
{
    QImage newImage(event->size(), QImage::Format_RGB32);
    newImage.fill(Qt::white);

    QImage newCanvas(event->size(), QImage::Format_ARGB32);
    newCanvas.fill(Qt::transparent);

    QPainter p(&newImage);
    p.drawImage(0, 0, image);

    p.end();

    image = newImage;
    canvas = newCanvas;
}

QImage PaintWidget::getImage() const
{
    return image;
}

void PaintWidget::setGradientField(GradientField* field)
{
    gradientField = field;
}
