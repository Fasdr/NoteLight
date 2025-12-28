#include <guiInternal.h>
#include <QTouchEvent>

PositionControl::PositionControl(QWidget* parent) : QWidget(parent),
        gridLayout(this) {
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    // std::array<QString, 9> directionElements = {
    //     "↖️", "⬆️", "↗️",
    //     "⬅️", "🔃", "➡️",
    //     "↙️", "⬇️", "↘️"};
    std::array<QString, 9> directionElements = {
        "↘", "⬇", "↙",
        "▶", "◆︎", "◀",
        "↗", "⬆", "↖"};
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
#include <format>
bool PositionControl::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::TouchBegin:
            break;
        case QEvent::TouchUpdate:
            break;
        case QEvent::TouchEnd:
            break;
        default:
            return QWidget::event(event);
    }
    QTouchEvent* touch = static_cast<QTouchEvent*>(event);
    std::cout << std::format("({}, {}), ({}, {})",
        rect().topLeft().x(), rect().topLeft().y(),
        rect().bottomRight().x(), rect().bottomRight().y()) << std::endl;
    std::cout << "Id: " << touch->points()[0].id() << std::endl;
    std::cout << "x: " << touch->points()[0].position().x() << " y: " << touch->points()[0].position().y() << std::endl;


    event->accept();
    return true;
}