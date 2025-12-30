#include <guiInternal.h>
#include <QPainter>
#include <QGuiApplication>
#include <QRectF>
#include <QColorDialog>

#include <algorithm>
#include <iostream>
#include <cmath>

WritingArea::WritingArea(QWidget* parent) : QWidget(parent),
        drawingToolsMenu(this),
        canvasImage(QGuiApplication::primaryScreen()->geometry().width(),
         QGuiApplication::primaryScreen()->geometry().height()),
         patchSize(std::max(QGuiApplication::primaryScreen()->geometry().width(),
                    QGuiApplication::primaryScreen()->geometry().height()) + 256) {
    setAttribute(Qt::WA_StaticContents);
    
    
    canvasImage.fill(canvasBackgroundColor);

    canvasPen = QPen(Qt::white, 4.0);

    // connect(&drawingToolsMenu, &DrawingToolsMenu::requireBackgroundColor,
    //         this, &WritingArea::updateBackgroundColor);

    connect(drawingToolsMenu.getPositionControl(), &PositionControl::scrollRequested,
            this, &WritingArea::updateScroll);

    connect(drawingToolsMenu.getBackgroundColorDialog(), &SimpleColorDialog::colorUpdated,
            this, &WritingArea::updateBackgroundColor);

    connect(drawingToolsMenu.getPenColorDialog(), &SimpleColorDialog::colorUpdated,
            this, &WritingArea::updatePenColor);

    connect(drawingToolsMenu.getPenWidthSliderDialog(), &PenWidthSliderDialog::penWidthChanged,
            this, &WritingArea::updatePenWidth);
    
    connect(drawingToolsMenu.getZoomControl(), &ZoomControl::zoomValueChanged,
            this, &WritingArea::updateZoom);
}

WritingArea::~WritingArea() {
    
}

DrawingToolsMenu* WritingArea::getDrawingToolsMenu() {
    return &drawingToolsMenu;
}

void WritingArea::tabletEvent(QTabletEvent *event) {
    event->accept();
    auto point = event->position();

    switch (event->type()) {
        case QEvent::TabletMove:
            processSegment(lastPoint, point);
            lastPoint = point;
            return;
        case QEvent::TabletPress:
            lastPoint = point;
            return;
        case QEvent::TabletRelease:
            processSegment(lastPoint, point);
            lastPoint = point;
            return;
        default:
            return;
    }
}

int WritingArea::processSegment(QPointF startPoint, QPointF endPoint) {
    
    QPainter canvasPainter(&canvasImage);
    canvasPainter.setRenderHint(QPainter::Antialiasing, true);
    canvasPainter.setPen(canvasPen);
    canvasPainter.drawLine(startPoint, endPoint);
    update(QRectF(startPoint, endPoint).normalized().adjusted(
        -canvasPen.widthF(), -canvasPen.widthF(), +canvasPen.widthF(), +canvasPen.widthF()).toRect());
    
    startPoint = startPoint / zoom + QPointF{xOrigin, yOrigin};
    endPoint = endPoint / zoom + QPointF{xOrigin, yOrigin};

    LineSegment thisSegment{
        startPoint, endPoint, canvasPen.color(), canvasPen.widthF() / zoom};

    auto [si, sj] = getCoordinates(startPoint);
    auto [ei, ej] = getCoordinates(endPoint);

    auto [miI, maI] = std::minmax(si, ei);
    auto [miJ, maJ] = std::minmax(sj, ej);
    for (int i{miI}; i <= maI; ++i) {
        for (int j{miJ}; j <= maJ; ++j) {
            internalStore[combineIntoIndex(i, j)].push_back(thisSegment);
        }
    }

    return 0;
}

// void WritingArea::mousePressEvent(QMouseEvent *event) {
//     if (event->button() == Qt::LeftButton) {
//         auto point = event->position().toPoint();
//         std::cout << std::format("x: {}, y: {}, rx: {}, ry: {}",
//             point.x(), point.y(), point.rx(), point.ry()) << std::endl;
//     }
// }

void WritingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, canvasImage);
}

void WritingArea::updateScroll(QPointF delta) {
    xOrigin += delta.x() / zoom;
    yOrigin += delta.y() / zoom;
    recreateCanvas();
}

void WritingArea::updateBackgroundColor(QColor newColor) {
    canvasBackgroundColor = newColor;
    recreateCanvas();
}

void WritingArea::updatePenWidth(int newWidth) {
    canvasPen.setWidth(newWidth);   
}

void WritingArea::updateZoom(int newZoomValue) {
    double newZoom{static_cast<double>(newZoomValue) / 100};
    if (newZoom != zoom) {
        QPointF newOrigin = QPointF{xOrigin, yOrigin} 
            + geometry().bottomRight().toPointF() * (1 / zoom - 1 / newZoom);
        xOrigin = newOrigin.x();
        yOrigin = newOrigin.y();
        zoom = newZoom;
        recreateCanvas();
    }
}

int WritingArea::recreateCanvas() {

    canvasImage.fill(canvasBackgroundColor);

    QPointF startCorner{xOrigin, yOrigin};
    QPointF endCorner = startCorner + QPointF{
        static_cast<double>(this->geometry().width()),
        static_cast<double>(this->geometry().height())} / zoom;

    auto [si, sj] = getCoordinates(startCorner);
    auto [ei, ej] = getCoordinates(endCorner);

    auto [miI, maI] = std::minmax(si, ei);
    auto [miJ, maJ] = std::minmax(sj, ej);
    
    QPainter canvasPainter(&canvasImage);
    canvasPainter.setRenderHint(QPainter::Antialiasing, true);
    QPen tempPen{canvasPen};
    canvasPainter.setPen(tempPen);

    for (int i{miI}; i <= maI; ++i) {
        for (int j{miJ}; j <= maJ; ++j) {
            int idx{combineIntoIndex(i, j)};
            if (internalStore.contains(idx)) {
                for (const LineSegment& thatSegment : internalStore[idx]) {
                    tempPen.setColor(thatSegment.color);
                    tempPen.setWidthF(thatSegment.width * zoom);
                    canvasPainter.setPen(tempPen);
                    canvasPainter.drawLine(
                        (thatSegment.start - startCorner) * zoom, 
                        (thatSegment.end - startCorner) * zoom);
                }
            }
        }
    }
    update();

    return 0;
}

void WritingArea::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (this->geometry().width() > canvasImage.width() 
            || this->geometry().height() > canvasImage.height()) {
        int extraMargin = 256;
        canvasImage = QPixmap(this->geometry().width() + extraMargin,
                                this->geometry().height() + extraMargin);
        recreateCanvas();
    }

    updateDrawingToolsMenuPosition();
}

void WritingArea::updateDrawingToolsMenuPosition() {
    int margin = 10;
    drawingToolsMenu.move(
        margin,
        this->height() - drawingToolsMenu.height() - margin);
}

void WritingArea::updatePenColor(QColor newColor) {
    canvasPen.setColor(newColor);
}

inline std::pair<int, int> WritingArea::getCoordinates(QPointF point) {
    return {static_cast<int>(std::floor(point.rx() / patchSize)),
             static_cast<int>(std::floor(point.ry() / patchSize))};
}

inline int WritingArea::combineIntoIndex(int i, int j) {
    int ni = (i + nPatches / 2);
    int nj = (j + nPatches / 2);
    return ni * nPatches + nj;
}