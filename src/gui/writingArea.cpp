#include <guiInternal.h>
#include <QPainter>
#include <QGuiApplication>

#include <algorithm>
#include <iostream>
#include <cmath>

WritingArea::WritingArea(QWidget* parent) : QWidget(parent),
        canvasImage(QGuiApplication::primaryScreen()->geometry().width(),
         QGuiApplication::primaryScreen()->geometry().height()),
         patchSize(std::max(QGuiApplication::primaryScreen()->geometry().width(),
                    QGuiApplication::primaryScreen()->geometry().height())) {
    setAttribute(Qt::WA_StaticContents);
    
    
    canvasImage.fill(qRgb(0, 0, 0));
    // std::cout << QGuiApplication::primaryScreen()->geometry().width() << std::endl;
    // std::cout << QGuiApplication::primaryScreen()->geometry().height() << std::endl;
    // std::cout << QGuiApplication::primaryScreen()->devicePixelRatio() << std::endl;
    // std::cout << devicePixelRatio() << std::endl;
    // std::cout << devicePixelRatioF() << std::endl;
    

    // QPainter canvasPainter(&canvasImage);
    // canvasPainter.setRenderHint(QPainter::Antialiasing, true);
    // QPen canvasPen(Qt::white, 4.0);
    // canvasPainter.setPen(canvasPen);
    // canvasPainter.drawLine(0, 0, QGuiApplication::primaryScreen()->geometry().width() / 2, 200);
    
    canvasPen = QPen(Qt::white, 4.0);
}

WritingArea::~WritingArea() {

}

void WritingArea::tabletEvent(QTabletEvent *event) {
    event->accept();
    auto point = event->position();

    switch (event->type()) {
        case QEvent::TabletMove:
            processSegment(lastPoint, point);
            lastPoint = point;
            return;
        case QEvent::TabletPress:
            lastPoint = point;
            return;
        case QEvent::TabletRelease:
            processSegment(lastPoint, point);
            lastPoint = point;
            return;
        default:
            return;
    }


    // std::cout << "Event type: " << event->type() << std::endl;
    // std::cout << std::format("x: {}, y: {}, rx: {}, ry: {}",
    //     point.x(), point.y(), point.rx(), point.ry()) << std::endl;
    // if (event->type)
}

int WritingArea::processSegment(QPointF startPoint, QPointF finishPoint) {

    QPainter canvasPainter(&canvasImage);
    canvasPainter.setRenderHint(QPainter::Antialiasing, true);
    canvasPainter.setPen(canvasPen);
    canvasPainter.drawLine(startPoint, finishPoint);
    
    return 0;
}



// void WritingArea::mousePressEvent(QMouseEvent *event) {
//     if (event->button() == Qt::LeftButton) {
//         auto point = event->position().toPoint();
//         std::cout << std::format("x: {}, y: {}, rx: {}, ry: {}",
//             point.x(), point.y(), point.rx(), point.ry()) << std::endl;
//     }
// }

void WritingArea::paintEvent(QPaintEvent *event) {
    // canvasImage = QPixmap(this->geometry().width(), this->geometry().height());
    // canvasImage.fill(qRgb(0, 0, 0));
    QPainter painter(this);
    painter.drawPixmap(0, 0, canvasImage);
    // std::cout << "W: " << this->geometry().width() << " H: " << this->geometry().height() << std::endl;
}

inline std::pair<int, int> WritingArea::getCoordinates(QPointF point) {
    return {static_cast<int>(std::floor(point.rx() / patchSize)),
             static_cast<int>(std::floor(point.ry() / patchSize))};
}