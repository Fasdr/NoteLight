#include <titleBar.h>

#include <QMouseEvent>
#include <QWindow>

TitleBar::TitleBar(QWidget* parent) : QFrame(parent),
        titleBarLayout(this) {
    
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setMidLineWidth(2);

    titleBarLayout.addStretch();
    
    titleBarLayout.addWidget(&exitButton);
    exitButton.setText("❌");
    exitButton.setFixedSize(60, 60);
    exitButton.setStyleSheet(
        "background-color: white;"
        "border: 1px solid black;"
        "border-radius: 30px;"
        "color: white;"
        "font-size: 25px;"
    );



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