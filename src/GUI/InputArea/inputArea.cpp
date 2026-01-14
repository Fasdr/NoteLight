#include <inputArea.h>

#include <QEvent>
#include <QTouchEvent>
#include <array>
#include <QPair>
#include <QPointF>

#include <QDebug>

InputArea::InputArea(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents, true);
}

InputArea::~InputArea() {

}

namespace {
    bool scrolling{false};
    std::array<QPair<int, QPointF>, 4> idPosArr; 
}

bool InputArea::event(QEvent* event) {
    QEvent::Type eventType{event->type()};
    if (eventType != QEvent::TouchBegin 
        && eventType != QEvent::TouchUpdate 
        && eventType != QEvent::TouchEnd) {
        return QWidget::event(event);
    }
    QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
    if (touchEvent->points().size() == 4) {
        if (scrolling) {
            QPointF currentMove{};
            int count{};
            for (const QTouchEvent::TouchPoint& touchPoint : touchEvent->points()) {
                for (int idx{}; idx < 4; ++idx) {
                    if (touchPoint.id() == idPosArr[idx].first) {
                        currentMove += touchPoint.position() - idPosArr[idx].second;
                        idPosArr[idx].second = touchPoint.position();
                        ++count;
                        break;
                    }
                }
            }
            if (count < 4) {
                scrolling = false;
            }
            qDebug() << currentMove / count;
        } else {
            int idx{-1};
            for (const QTouchEvent::TouchPoint& touchPoint : touchEvent->points()) {
                idPosArr[++idx] = {touchPoint.id(), touchPoint.position()};
            }
            scrolling = true;
            // TODO: implement required distance?
            // for (int i{}; i < 3; ++i) {
            //     for (int j{i + 1}; j < 4; ++j) {

            //     }
            // }
        }
    } else {
        scrolling = false;
    }
    

    event->accept();
    return true;
}