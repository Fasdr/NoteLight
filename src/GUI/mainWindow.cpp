#include <mainWindow.h>

#include <QMouseEvent>
#include <QWindow>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent, Qt::FramelessWindowHint),
                                            titleBar(this) {
    setMouseTracking(true);

    connect(&titleBar.exitButton, &QPushButton::clicked,
            this, &MainWindow::exitApp);
}

MainWindow::~MainWindow() {

}

void MainWindow::resizeEvent(QResizeEvent* event) {
    // Sets up the title bar size and its position
    quint32 titleBarMargin{10};
    QSize titleBarSize{size()};
    // TODO: Should be based on the font size for the app
    titleBarSize.setWidth(titleBarSize.width() - 2 * titleBarMargin);
    titleBarSize.setHeight(80);
    titleBar.setFixedSize(titleBarSize);
    titleBar.move(titleBarMargin, titleBarMargin);
}

namespace {
    quint32 resizeMargin{5};

    Qt::Edges getEdges(const QPointF& position, const MainWindow* mainWindow) {
        Qt::Edges edges = Qt::Edges();
        if (mainWindow->isMaximized() || mainWindow->isFullScreen()) {
            return edges;
        }

        if (position.x() >= 0 && position.x() < resizeMargin) {
            edges |= Qt::LeftEdge;
        } else if (mainWindow->width() - position.x() < resizeMargin 
                    && position.x() <= mainWindow->width()) {
            edges |= Qt::RightEdge;
        }

        if (position.y() >= 0 && position.y() < resizeMargin) {
            edges |= Qt::TopEdge;
        } else if (mainWindow->height() - position.y() < resizeMargin
                    && position.y() <= mainWindow->height()) {
            edges |= Qt::BottomEdge;
        }

        return edges;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        Qt::Edges edges = getEdges(event->position(), this);
        if (edges != Qt::Edges()) {
            windowHandle()->startSystemResize(edges);
        }
    }
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    Qt::Edges edges = getEdges(event->position(), this);
    if (edges == Qt::Edges()) {
        setCursor(Qt::ArrowCursor);
    } else if ((edges & Qt::LeftEdge) && (edges & Qt::TopEdge)) {
        setCursor(Qt::SizeFDiagCursor);
    } else if ((edges & Qt::RightEdge) && (edges & Qt::BottomEdge)) {
        setCursor(Qt::SizeFDiagCursor);
    } else if ((edges & Qt::RightEdge) && (edges & Qt::TopEdge)) {
        setCursor(Qt::SizeBDiagCursor);
    } else if ((edges & Qt::LeftEdge) && (edges & Qt::BottomEdge)) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (edges & Qt::LeftEdge || edges & Qt::RightEdge) {
        setCursor(Qt::SizeHorCursor);
    } else if (edges & Qt::TopEdge || edges & Qt::BottomEdge) {
        setCursor(Qt::SizeVerCursor);
    }
    QWidget::mouseMoveEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    exitApp();
}

void MainWindow::exitApp() {
    qApp->exit();
}