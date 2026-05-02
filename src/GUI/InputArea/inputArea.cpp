#include <inputArea.h>

#include <QEvent>
#include <QPair>
#include <QPointF>
#include <QTouchEvent>

#include <QDebug>
#include <qcontainerfwd.h>
#include <qevent.h>
#include <qlogging.h>
#include <qwidget.h>

namespace {
    // canvas related parameters
    double horizontalProp{1};
    double verticalProp{2};
    double verticalShift{0};
    int currentPageNumber{0};
    double pageWidth{};
    double pageHeight{};
    // writing related parameters
    struct Stroke {
        QVector<QPointF> points;
    };
    struct Page {
        QVector<Stroke> strokes;
    };
    struct Document {
        QVector<Page> pages;
    };
    Document document;
}

InputArea::InputArea(QWidget* parent) : QWidget(parent) {
    //    setAttribute(Qt::WA_AcceptTouchEvents, true);
}

void InputArea::tabletEvent(QTabletEvent* event) {
    event->accept();

    QPointF point{event->position()};
    double xPos{point.x() / pageWidth};
    double yPos{verticalShift + point.y() / pageHeight};
    int onPage{currentPageNumber};
    if (yPos >= 1) {
        ++onPage;
        yPos -= 1;
    }
    static int strokePage{};
    static bool valid{};
    static Stroke stroke;

    switch (event->type()) {
    case QEvent::TabletPress:
        strokePage = onPage;
        valid = true;
        stroke.points.emplaceBack(xPos, yPos);
        return;
    case QEvent::TabletMove:
        if (strokePage != onPage) {
            valid = false;
        }
        if (valid) {
            stroke.points.emplaceBack(xPos, yPos);
        }
        return;
    case QEvent::TabletRelease:
        if (strokePage != onPage) {
            valid = false;
        }
        if (valid) {
            stroke.points.emplaceBack(xPos, yPos);
            for (auto p : stroke.points) {
                qDebug() << p;
            }
        }
        return;
    default:
        return;
    }
}

void InputArea::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    pageWidth = this->geometry().width();
    pageHeight = pageWidth * verticalProp / horizontalProp;
}

InputArea::~InputArea() {}
