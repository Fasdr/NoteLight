#include <titleBar.h>

#include <QMouseEvent>
#include <QWindow>

TitleBar::TitleBar(QWidget* parent) : QFrame(parent),
        titleBarLayout(this) {    
    
    titleBarLayout.setContentsMargins(5, 0, 5, 0);
    titleBarLayout.setAlignment(Qt::AlignVCenter);
    titleBarLayout.addStretch();
    titleBarLayout.addWidget(&fullScreenButton);
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