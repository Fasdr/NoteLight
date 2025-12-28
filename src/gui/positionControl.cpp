#include <guiInternal.h>
#include <QTouchEvent>

#include <cmath>

PositionControl::PositionControl(QWidget* parent) : QWidget(parent),
        gridLayout(this)  {
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
    QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
    bool keepScrolling{false};
    if (scrolling) {
        for (const QTouchEvent::TouchPoint& touchPoint : touchEvent->points()) {
            if (touchPoint.id() == fingerId && rect().contains(touchPoint.position().toPoint())) {
                keepScrolling = true;
                emit scrollRequested(applyAcceleration(touchPoint.position()));
                previousPoint = touchPoint.position();
                break;
            }
        }
        if (!keepScrolling) {
            fingerId = -1;
            scrolling = false;
        }
    } else {
        for (const auto& touchPoint : touchEvent->points()) {
            // if (touchPoint.state() == QEventPoint::Pressed && rect().contains(touchPoint.position().toPoint())) {
            if (rect().contains(touchPoint.position().toPoint())) {
                scrolling = true;
                fingerId = touchPoint.id();
                previousPoint = touchPoint.position();
                traveledDistance = 0.0;
                break;
            }
        }
    }
    event->accept();
    return true;
}

#include <iostream>
QPointF PositionControl::applyAcceleration(QPointF newPoint) {
    QPointF delta(newPoint - previousPoint);
    traveledDistance += std::pow(std::pow(delta.x(), 2) + std::pow(delta.y(), 2), 0.5);
    double prop = traveledDistance / rect().width();
    if (prop < 0.25) {
        return delta * prop;
    } else if (prop < 0.5) {
        return delta;
    } else {
        return delta * (16 * prop - 7);
    }
}