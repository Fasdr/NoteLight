#include <guiInternal.h>

#include <iostream>

WritingArea::WritingArea(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
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