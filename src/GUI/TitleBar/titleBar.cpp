#include <titleBar.h>

#include <QMouseEvent>

TitleBar::TitleBar(QWidget* parent) : QFrame(parent) {
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setMidLineWidth(2);
}

TitleBar::~TitleBar() {

}

void TitleBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit sendStartDraggingPosition(event->globalPosition());
        // dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void TitleBar::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        emit sendNewDraggingPosition(event->globalPosition());
        // move(event->globalPos() - dragPosition);
        event->accept();
    }
}