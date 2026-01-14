#include <inputArea.h>

#include <QEvent>
#include <QTouchEvent>

#include <QDebug>

InputArea::InputArea(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents, true);
}

InputArea::~InputArea() {

}

bool InputArea::event(QEvent* event) {
    QEvent::Type eventType{event->type()};
    if (eventType != QEvent::TouchBegin 
        && eventType != QEvent::TouchUpdate 
        && eventType != QEvent::TouchEnd) {
        return QWidget::event(event);
    }
    QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
    if (touchEvent->points().size() >= 4) {
        for (const QTouchEvent::TouchPoint& touchPoint : touchEvent->points()) {
            qDebug() << touchPoint;
        }
    }
    

    event->accept();
    return true;
}