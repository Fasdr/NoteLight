#include <inputArea.h>

#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QPair>
#include <QPixmap>
#include <QPointF>
#include <QTouchEvent>
#include <QWidget>

#include <algorithm>
#include <qcontainerfwd.h>
#include <qicon.h>
#include <qlogging.h>
#include <utility>

namespace {
    // canvas related parameters
    double horizontalProp{1};
    double verticalProp{2};
    double verticalShift{0};
    double verticalSeparator{0.05};
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

    document.pages.push_back(Page{});
}

void InputArea::tabletEvent(QTabletEvent* event) {
    event->accept();

    QPointF point{event->position()};
    double xPos{point.x() / pageWidth};
    double yPos{verticalShift + point.y() / pageHeight};
    int onPage{currentPageNumber};
    if (yPos >= 1) {
        ++onPage;
        yPos -= 1 + verticalSeparator;
        if (yPos < 0) {
            yPos = 0;
        }
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
            if (onPage < document.pages.size()) {
                document.pages[onPage].strokes.push_back(std::move(stroke));
                // TODO: actully draw it
            }
        }
        stroke.points.clear();
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

QPixmap InputArea::preparePage(int pageNumber) {
    QPixmap newPage(pageWidth, pageHeight);
    QPainter pagePainter(&newPage);
    // TODO: add actual settings for pages
    newPage.fill();
    for (Stroke curStroke : document.pages[pageNumber].strokes) {
        // TODO: different styles for this line
        QVector<QPointF> points{curStroke.points};
        std::for_each(points.begin(), points.end(), [](QPointF& point) {
            point.rx() *= pageWidth;
            point.ry() *= pageHeight;
        });
        pagePainter.drawPolyline(points.constData(), points.size());
    }
    return newPage;
}

void InputArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    int pageCount = document.pages.size();
    double pageBeginning{-verticalShift * pageHeight};
    int inputHeight{this->geometry().height()};
    for (int pageNumber{currentPageNumber};
         pageNumber < pageCount && pageBeginning < inputHeight; ++pageNumber) {
        QPixmap curPage(preparePage(pageNumber));
        painter.drawPixmap(0, pageBeginning, curPage);
        pageBeginning += pageHeight * (1 + verticalSeparator);
    }
}

InputArea::~InputArea() {}
