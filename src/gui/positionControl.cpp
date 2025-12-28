#include <guiInternal.h>
#include <QTouchEvent>

PositionControl::PositionControl(QWidget* parent) : QWidget(parent),
        gridLayout(this) {
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    std::array<QString, 9> directionElements = {
        "↖️", "⬆️", "↗️",
        "⬅️", "🔃", "➡️",
        "↙️", "⬇️", "↘️"};
    for (int i{0}; i < 9; ++i) {
        directionLabels[i].setText(directionElements[i]);
        directionLabels[i].setFixedSize(80, 80);
        directionLabels[i].setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        directionLabels[i].setAlignment(Qt::AlignCenter);
        gridLayout.addWidget(&directionLabels[i], i / 3, i % 3);
    }
    for (int i{0}; i < 9; ++i) {
        gridLayout.setColumnStretch(i, 1);
        gridLayout.setRowStretch(i, 1);
    }

}

PositionControl::~PositionControl() {

}

#include <iostream>
bool PositionControl::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::TouchBegin:
            std::cout << "TouchBegin" << std::endl;
            QTouchEvent* touch = static_cast<QTouchEvent*>(event);
            std::cout << touch->points().size() << std::endl;
            break;
        case QEvent::TouchUpdate:
            std::cout << "TouchUpdate" << std::endl;
            QTouchEvent* touch = static_cast<QTouchEvent*>(event);
            std::cout << touch->points().size() << std::endl;
            break;
        case QEvent::TouchEnd:
            std::cout << "TouchEnd" << std::endl;
            QTouchEvent* touch = static_cast<QTouchEvent*>(event);
            std::cout << touch->points().size() << std::endl;
            break;
        default:
            return QWidget::event(event);
    }
    event->accept();
    return true;
}