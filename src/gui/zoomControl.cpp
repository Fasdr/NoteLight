#include <guiInternal.h>
#include <QIcon>
#include <QStyle>
#include <QApplication>

ZoomControl::ZoomControl(QWidget* parent) : QWidget(parent), zoomLayout(this),
    increaseButton("➕", this), zoomButton("🔍100%", this), decreaseButton("➖", this) {
    zoomLayout.setContentsMargins(0, 0, 0, 0);
    zoomLayout.setSpacing(0);
    
    zoomLayout.addWidget(&decreaseButton);
    connect(&decreaseButton, &QPushButton::clicked,
        this, &ZoomControl::decreaseZoomClicked);
    zoomLayout.addWidget(&zoomButton);
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