#include <cstddef>
#include <deque>
#include <inputArea.h>

#include <QDebug>
#include <QEvent>
#include <QMap>
#include <QPainter>
#include <QPair>
#include <QPixmap>
#include <QPointF>
#include <QTouchEvent>
#include <QWidget>

#include <algorithm>
#include <qcontainerfwd.h>
#include <qcoreevent.h>
#include <qicon.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <utility>

namespace {
    // canvas related parameters
    double horizontalProp{1};
    double verticalProp{2};
    double verticalShift{0};
    double verticalSeparator{0.02};
    int currentPageNumber{0};
    double pageWidth{};
    double pageHeight{};
    double windowHeight{};
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

namespace {
    // undo/redo

    template <class T> class EvictingStack {
    private:
        std::deque<T> data;
        std::size_t maxSize{};

    public:
        EvictingStack(std::size_t maxSize) : maxSize(maxSize) {}
        bool const empty() {
            return data.empty();
        }
        T pop() {
            T tmp{std::move(data.back())};
            data.pop_back();
            return tmp;
        }
        void push(const T& elem) {
            data.push_back(elem);
            if (data.size() > maxSize) {
                data.pop_front();
            }
        }
        void push(T&& elem) {
            data.push_back(std::move(elem));
            if (data.size() > maxSize) {
                data.pop_front();
            }
        }
    };

    int maxHistorySize{200};
    EvictingStack<int> entriesHistory(maxHistorySize);
    EvictingStack<QPair<int, Stroke>> storedChanges(maxHistorySize);
}

InputArea::InputArea(QWidget* parent) : QWidget(parent) {
    //    setAttribute(Qt::WA_AcceptTouchEvents, true);

    document.pages.push_back(Page{});
}

namespace {
    QPen currentStrokePen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap,
                          Qt::RoundJoin);
    StrokeType currentStrokeType{StrokeType::PolyLine};
    double currentStrokePropWidth{0.005};
}

namespace {
    void processSelection(int onPage, Stroke stroke) {}
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
    static bool selectionMode{};
    static Stroke stroke;

    auto actualEvent{event->type()};
    bool selecting{(event->buttons() & Qt::MiddleButton) != 0};
    if (selecting) {
        selectionMode = true;
    }
    if (actualEvent == QEvent::TabletRelease &&
        event->buttons() != Qt::NoButton) {
        actualEvent = QEvent::TabletMove;
    }

    switch (actualEvent) {
    case QEvent::TabletPress:
        strokePage = onPage;
        valid = true;
        selectionMode = false;
        stroke.points.emplaceBack(xPos, yPos);
        // set up stroke
        stroke.type = currentStrokeType;
        stroke.pen = currentStrokePen;
        stroke.propWidth = currentStrokePropWidth;
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
                if (selectionMode) {
                    processSelection(onPage, std::move(stroke));
                } else {
                    if (storedPixmaps.contains(onPage)) {
                        stroke.addToPixmap(storedPixmaps[onPage]);
                    } else {
                        QPixmap curPage(preparePage(onPage));
                        stroke.addToPixmap(curPage);
                        storedPixmaps[onPage] = std::move(curPage);
                    }
                    document.pages[onPage].strokes.push_back(std::move(stroke));
                    entriesHistory.push(onPage);
                    // TODO: actully draw it with a targeted update?!
                    update();
                }
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
    windowHeight = this->geometry().height();
    pageHeight = pageWidth * verticalProp / horizontalProp;
    setScrollBar();
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

void InputArea::undoLast() {
    if (entriesHistory.empty()) {
        return;
    }
    int lastPage{entriesHistory.pop()};
    storedChanges.push({lastPage, document.pages[lastPage].strokes.back()});
    document.pages[lastPage].strokes.pop_back();
    if (auto it{storedPixmaps.find(lastPage)}; it != storedPixmaps.end()) {
        storedPixmaps.erase(storedPixmaps.find(lastPage));
    }
    update();
}

void InputArea::redoLast() {
    if (storedChanges.empty()) {
        return;
    }
    auto [lastPage, stroke] = storedChanges.pop();
    if (storedPixmaps.contains(lastPage)) {
        stroke.addToPixmap(storedPixmaps[lastPage]);
    }
    document.pages[lastPage].strokes.push_back(std::move(stroke));
    entriesHistory.push(lastPage);
    update();
}

void InputArea::getScroll(int val) {
    double fullShift{val / 100.0};
    currentPageNumber = fullShift / (1 + verticalSeparator);
    if (currentPageNumber < 0) {
        currentPageNumber = 0;
    }
    verticalShift = fullShift - currentPageNumber * (1 + verticalSeparator);
    update();
}

void InputArea::setScrollBar() {
    int oneFullStep{100};
    int extraSpaceTop = -oneFullStep * verticalSeparator;
    int extraSpacePage = (1 + verticalSeparator) * oneFullStep -
                         windowHeight * oneFullStep / pageHeight;
    extraSpacePage = std::max(0, extraSpacePage);
    int extraSpaceBottom =
        oneFullStep * ((document.pages.size() - 1) * (1 + verticalSeparator)) +
        extraSpacePage;
    emit signalScrollBar(extraSpaceTop, extraSpaceBottom, oneFullStep);
}

void InputArea::newPage() {
    document.pages.push_back(Page{});
    setScrollBar();
    update();
}

void InputArea::setNewPen(QPen newPen, double newPropWidth) {
    currentStrokePen = newPen;
    currentStrokePropWidth = newPropWidth;
}
