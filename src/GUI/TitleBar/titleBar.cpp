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
    exitButton.setFixedSize(50, 50);
    // exitButton.setStyleSheet(
    //     "background-color: white;"
    //     "border: 1px solid black;"
    //     "border-radius: 25px;"
    //     "color: white;"
    //     "font-size: 25px;"
    // );

    exitButton.setStyleSheet(
        // NORMAL STATE
        "QPushButton {"
        "    background-color: white;"      
        "    border: 2px solid #e0e0e0;"    // Light grey border
        "    border-radius: 25px;"          // Perfectly circular
        "    color: #555555;"               // Dark grey text (visible!)
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    padding-bottom: 2px;"          // Visual correction for text alignment
        "}"
        
        // HOVER STATE (Mouse over)
        "QPushButton:hover {"
        "    background-color: #ff6b6b;"    // Turns Soft Red
        "    color: white;"                 // Text turns white
        "    border: 2px solid #ff6b6b;"    // Border matches background
        "}"
        
        // PRESSED STATE (Clicked)
        "QPushButton:pressed {"
        "    background-color: #ee5253;"    // Turns Darker Red
        "    border: 2px solid #ee5253;"
        "    padding-top: 2px;"             // "Pressing down" effect
        "    padding-bottom: 0px;"
        "}"
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