#include <guiInternal.h>
#include <QPainter>
#include <QGuiApplication>

#include <iostream>

WritingArea::WritingArea(QWidget* parent) : QWidget(parent),
        canvasImage(QGuiApplication::primaryScreen()->geometry().width(),
         QGuiApplication::primaryScreen()->geometry().height()) {
    setAttribute(Qt::WA_StaticContents);
    std::cout << QGuiApplication::primaryScreen()->geometry().width() << std::endl;
    std::cout << QGuiApplication::primaryScreen()->geometry().height() << std::endl;
    canvasImage.fill(qRgb(0, 0, 0));
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