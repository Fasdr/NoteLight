#include <guiInternal.h>
#include <QPainter>
#include <QGuiApplication>



#include <iostream>

WritingArea::WritingArea(QWidget* parent) : QWidget(parent),
        canvasImage(QGuiApplication::primaryScreen()->geometry().width() * 2,
         QGuiApplication::primaryScreen()->geometry().height()) {
    setAttribute(Qt::WA_StaticContents);
    canvasImage.fill(qRgb(0, 0, 0));
    // canvasImage.setDevicePixelRatio(2.0);
    std::cout << QGuiApplication::primaryScreen()->geometry().width() << std::endl;
    std::cout << QGuiApplication::primaryScreen()->geometry().height() << std::endl;
    std::cout << QGuiApplication::primaryScreen()->devicePixelRatio() << std::endl;
    std::cout << devicePixelRatio() << std::endl;
    std::cout << devicePixelRatioF() << std::endl;

    QPainter canvasPainter(&canvasImage);
    canvasPainter.setRenderHint(QPainter::Antialiasing, true);
    QPen canvasPen(Qt::white, 4.0);
    canvasPainter.setPen(canvasPen);
    canvasPainter.drawLine(0, 0, QGuiApplication::primaryScreen()->geometry().width() / 2, 200);

}

WritingArea::~WritingArea() {

}

void WritingArea::tabletEvent(QTabletEvent *event) {
    event->accept();
    auto point = event->position().toPoint();
    std::cout << "Event type: " << event->type() << std::endl;
    std::cout << std::format("x: {}, y: {}, rx: {}, ry: {}",
        point.x(), point.y(), point.rx(), point.ry()) << std::endl;
    // if (event->type)
}

// void WritingArea::mousePressEvent(QMouseEvent *event) {
//     if (event->button() == Qt::LeftButton) {
//         auto point = event->position().toPoint();
//         std::cout << std::format("x: {}, y: {}, rx: {}, ry: {}",
//             point.x(), point.y(), point.rx(), point.ry()) << std::endl;
//     }
// }

void WritingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, canvasImage);
}