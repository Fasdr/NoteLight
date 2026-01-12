#include <titleBar.h>

#include <QMouseEvent>
#include <QWindow>

TitleBar::TitleBar(QWidget* parent) : QFrame(parent),
        titleBarLayout(this) {    
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setMidLineWidth(2);
    
    titleBarLayout.setContentsMargins(5, 0, 5, 0);
    titleBarLayout.setAlignment(Qt::AlignVCenter);
    titleBarLayout.addStretch();
    titleBarLayout.addWidget(&minimizeButton);
    titleBarLayout.addWidget(&maximizeButton);
    titleBarLayout.addWidget(&exitButton);
}

TitleBar::~TitleBar() {

}

void TitleBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QWindow *window = this->window()->windowHandle();
        if (window) {
            bool success = window->startSystemMove();
        }
        event->accept();
    }
}