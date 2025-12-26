#include <guiInternal.h>
#include <QIcon>
#include <QStyle>
#include <QApplication>

ZoomControl::ZoomControl(QWidget* parent) : QWidget(parent), zoomLayout(this),
    increaseButton(this), zoomButton(this), decreaseButton(this) {
    zoomLayout.setContentsMargins(0, 0, 0, 0);
    zoomLayout.setSpacing(0);
    
    // QIcon arrowDownIcon = QApplication::style()->standardIcon(QStyle::SP_ArrowDown);
    decreaseButton.setText("➖");
    zoomLayout.addWidget(&decreaseButton);
    connect(&decreaseButton, &QPushButton::clicked,
        this, &ZoomControl::decreaseZoomClicked);

    zoomButton.setText("🔍100%");
    zoomLayout.addWidget(&zoomButton);

    // QIcon arrowUpIcon = QApplication::style()->standardIcon(QStyle::SP_ArrowUp);
    increaseButton.setText("➕");
    zoomLayout.addWidget(&increaseButton);
    connect(&increaseButton, &QPushButton::clicked,
        this, &ZoomControl::increaseZoomClicked);
  
}

ZoomControl::~ZoomControl() {

}

void ZoomControl::decreaseZoomClicked() {
    if (zoomValue == minVal) {
        return;
    }
    setZoomValue(zoomValue - stepSize);
}

void ZoomControl::increaseZoomClicked() {
    if (zoomValue == maxVal) {
        return;
    }
    setZoomValue(zoomValue + stepSize);
}

void ZoomControl::setZoomValue(int newZoomValue) {
    if (zoomValue == newZoomValue) {
        return;
    }
    zoomValue = newZoomValue;
    emit zoomValueChanged(newZoomValue);
    zoomButton.setText(QString("🔍%1%").arg(zoomValue, 3, 10, u' '));

    
}