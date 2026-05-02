#include <inputArea.h>

#include <QEvent>
#include <QPair>
#include <QPointF>
#include <QTouchEvent>
#include <array>

#include <QDebug>

InputArea::InputArea(QWidget* parent) : QWidget(parent) {
    //    setAttribute(Qt::WA_AcceptTouchEvents, true);
}

InputArea::~InputArea() {}
