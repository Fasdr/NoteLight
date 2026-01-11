#include <mainWindow.h>


MainWindow::MainWindow(QWidget* parent) : QWidget(parent, Qt::FramelessWindowHint),
    titleBar(this) {
    

    connect(&titleBar, &TitleBar::sendStartDraggingPosition,
            this, &MainWindow::acceptStartDraggingPosition);

    connect(&titleBar, &TitleBar::sendNewDraggingPosition,
            this, &MainWindow::acceptNewDraggingPosition);    
}

MainWindow::~MainWindow() {

}

void MainWindow::resizeEvent(QResizeEvent* event) {
    int margin{5};
    QSize titleBarSize{size()};
    // TODO: Should be based on the font size for the app
    titleBarSize.setHeight(40);
    titleBarSize.setWidth(titleBarSize.width() - 2 * margin);
    titleBar.setFixedSize(titleBarSize);
    titleBar.move(margin, margin);
}

namespace {
    QPointF dragPosition;
}

void MainWindow::acceptStartDraggingPosition(QPointF startPosition) {
    raise();
    dragPosition = startPosition - frameGeometry().topLeft();
}

void MainWindow::acceptNewDraggingPosition(QPointF newPosition) {
    move((newPosition - dragPosition).toPoint());
}