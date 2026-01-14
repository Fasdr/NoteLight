#include <mainWindow.h>

#include <QMouseEvent>
#include <QWindow>
#include <QFont>
#include <QFontDatabase>
#include <QWindowStateChangeEvent>

#include <QDebug>

void initMyResources() {
    Q_INIT_RESOURCE(resources);
}

namespace {
    constexpr int pixelSize{40};
    inline QFont createPixelFont(int fontSize) {
        QFont font("FreeSans");
        font.setPixelSize(fontSize);
        return font;
    }
    QFont appFont{createPixelFont(pixelSize)};

    inline QFont getIconFont() {
        int fontId{QFontDatabase::addApplicationFont(":/assets/fonts/MaterialIcons-Regular.ttf")};
        QString familyName;
        if (fontId == -1) {
            qDebug() << "Error: Could not load Material Icons!";
            return QFont();
        }
        return QFont(familyName);
    }
    QFont iconFont;
    QString appTheme("Dark");

    constinit QMap<QString, QVariant> parameters;
}



MainWindow::MainWindow(QWidget* parent) : QWidget(parent, Qt::FramelessWindowHint),
                                            titleBar(this), inputArea(this) {
    
    
    // setMouseTracking(true);
    inputArea.stackUnder(&titleBar);
    
    // TODO: strange behavior fullscreen + maximize + fullscreen = no state
    connect(&titleBar.fullScreenButton, &QPushButton::clicked,
            this, [this]() { this->setWindowState(Qt::WindowFullScreen ^ this->windowState()); });

    connect(&titleBar.minimizeButton, &QPushButton::clicked,
            this, [this]() { this->setWindowState(Qt::WindowMinimized ^ this->windowState()); });

    connect(&titleBar.maximizeButton, &QPushButton::clicked,
            this, [this]() { this->setWindowState(Qt::WindowMaximized ^ this->windowState()); });

    connect(&titleBar.exitButton, &QPushButton::clicked,
            this, &MainWindow::exitApp);
    

    iconFont = getIconFont();
    // Sets up visual parameters to use in this app
    
    iconFont.setPixelSize(appFont.pixelSize());
    parameters["appFont"] = appFont;
    parameters["iconFont"] = iconFont;
    parameters["appTheme"] = appTheme;
    setVisuals(parameters);
}

MainWindow::~MainWindow() {

}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent *stateEvent = static_cast<QWindowStateChangeEvent*>(event);
        // qDebug() << "Old state:" << stateEvent->oldState() << "New state:" << this->windowState();
        if ((stateEvent->oldState() & Qt::WindowMaximized) != (this->windowState() & Qt::WindowMaximized)) {
            titleBar.setMaximizedButton(this->windowState() & Qt::WindowMaximized);
        }
        if ((stateEvent->oldState() & Qt::WindowFullScreen) != (this->windowState() & Qt::WindowFullScreen)) {
            titleBar.setFullScreenButton(this->windowState() & Qt::WindowFullScreen);
        }
    }
    
    QWidget::changeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    // Sets up the title bar size and its position
    int titleBarMargin{10};
    QSize titleBarSize{size()};
    titleBarSize.setWidth(titleBarSize.width() - 2 * titleBarMargin);
    titleBarSize.setHeight(iconFont.pixelSize() * 1.75);
    titleBar.setFixedSize(titleBarSize);
    titleBar.move(titleBarMargin, titleBarMargin);

    // Sets up the input area
    inputArea.setFixedSize(size());
    inputArea.move(0, 0);

    QWidget::resizeEvent(event);
}

namespace {
    int resizeMargin{10};

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

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        Qt::Edges edges = getEdges(event->position(), this);
        if (edges != Qt::Edges()) {
            windowHandle()->startSystemResize(edges);
        }
    }
    QWidget::mousePressEvent(event);
}

// void MainWindow::mouseMoveEvent(QMouseEvent *event) {
//     Qt::Edges edges = getEdges(event->position(), this);
//     if (edges == Qt::Edges()) {
//         setCursor(Qt::ArrowCursor);
//     } else if ((edges & Qt::LeftEdge) && (edges & Qt::TopEdge)) {
//         setCursor(Qt::SizeFDiagCursor);
//     } else if ((edges & Qt::RightEdge) && (edges & Qt::BottomEdge)) {
//         setCursor(Qt::SizeFDiagCursor);
//     } else if ((edges & Qt::RightEdge) && (edges & Qt::TopEdge)) {
//         setCursor(Qt::SizeBDiagCursor);
//     } else if ((edges & Qt::LeftEdge) && (edges & Qt::BottomEdge)) {
//         setCursor(Qt::SizeBDiagCursor);
//     } else if (edges & Qt::LeftEdge || edges & Qt::RightEdge) {
//         setCursor(Qt::SizeHorCursor);
//     } else if (edges & Qt::TopEdge || edges & Qt::BottomEdge) {
//         setCursor(Qt::SizeVerCursor);
//     }
//     QWidget::mouseMoveEvent(event);
// }

void MainWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    exitApp();
}

void MainWindow::exitApp() {
    qApp->exit();
}