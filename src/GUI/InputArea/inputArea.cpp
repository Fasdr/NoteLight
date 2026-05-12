#include <inputArea.h>

#include <QDebug>
#include <QEvent>
#include <QMap>
#include <QPainter>
#include <QPair>
#include <QPen>
#include <QPixmap>
#include <QPointF>
#include <QTouchEvent>
#include <QWidget>

#include <algorithm>
#include <qcontainerfwd.h>
#include <qicon.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpixmap.h>
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
    enum class StrokeType { PolyLine };
    struct Stroke {
        StrokeType type;
        QPen pen;
        double propWidth;
        QVector<QPointF> points;

        void addToPixmap(QPixmap& pixmap) {
            switch (type) {
            case StrokeType::PolyLine: {
                QPainter painter(&pixmap);
                painter.setRenderHint(QPainter::Antialiasing, true);
                addToPainter(painter);
                break;
            }
            }
        }

        void addToPainter(QPainter& painter) {
            switch (type) {
            case StrokeType::PolyLine: {
                QVector<QPointF> points{this->points};
                std::for_each(points.begin(), points.end(), [](QPointF& point) {
                    point.rx() *= pageWidth;
                    point.ry() *= pageHeight;
                });
                pen.setWidthF(propWidth * pageWidth);
                painter.setPen(pen);
                painter.drawPolyline(points.constData(), points.size());
                break;
            }
            }
        }
    };
    struct Page {
        QVector<Stroke> strokes;
    };
    struct Document {
        QVector<Page> pages;
    };
    Document document;
    QMap<int, QPixmap> storedPixmaps;
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
    while (yPos >= 1) {
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
        // set up stroke
        stroke.type = StrokeType::PolyLine;
        stroke.pen =
            QPen(Qt::green, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        stroke.propWidth = 0.005;
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
                if (storedPixmaps.contains(onPage)) {
                    stroke.addToPixmap(storedPixmaps[onPage]);
                } else {
                    QPixmap curPage(preparePage(onPage));
                    stroke.addToPixmap(curPage);
                    storedPixmaps[onPage] = std::move(curPage);
                }
                document.pages[onPage].strokes.push_back(std::move(stroke));
                // TODO: actully draw it with targeted update?!
                update();
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
    storedPixmaps.clear();
}

QPixmap InputArea::preparePage(int pageNumber) {
    QPixmap newPage(pageWidth, pageHeight);
    QPainter pagePainter(&newPage);
    pagePainter.setRenderHint(QPainter::Antialiasing, true);
    // TODO: add actual settings for pages
    newPage.fill();
    for (Stroke& curStroke : document.pages[pageNumber].strokes) {
        // TODO: different styles for this line
        curStroke.addToPainter(pagePainter);
    }
    return newPage;
}

void InputArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int pageCount = document.pages.size();
    double pageBeginning{-verticalShift * pageHeight};
    int inputHeight{this->geometry().height()};
    for (int pageNumber{currentPageNumber};
         pageNumber < pageCount && pageBeginning < inputHeight; ++pageNumber) {
        if (storedPixmaps.contains(pageNumber)) {
            painter.drawPixmap(0, pageBeginning, storedPixmaps[pageNumber]);
        } else {
            QPixmap curPage(preparePage(pageNumber));
            painter.drawPixmap(0, pageBeginning, curPage);
            storedPixmaps[pageNumber] = std::move(curPage);
        }
        pageBeginning += pageHeight * (1 + verticalSeparator);
    }
}

InputArea::~InputArea() {}

void InputArea::setScrollBar() {
    emit signalScrollBar(
        0, 100 * document.pages.size() * (1 + verticalSeparator), 100);
}

void InputArea::getScroll(int val) {
    double fullShift{val / 100.0};
    currentPageNumber = fullShift / (1 + verticalSeparator);
    verticalShift = fullShift - currentPageNumber * (1 + verticalSeparator);
    update();
}

void InputArea::newPage() {
    document.pages.push_back(Page{});
    setScrollBar();
    update();
}
