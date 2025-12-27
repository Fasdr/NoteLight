#include <guiInternal.h>

SimpleSliderDialog::SimpleSliderDialog(int minVal, int maxVal, int step, QWidget* parent) : QDialog(parent),
        sliderControl(Qt::Horizontal, this) {
    sliderControl.setMinimum(minVal);
    sliderControl.setMaximum(maxVal);
    sliderControl.setSingleStep(step);
    sliderControl.setTickPosition(QSlider::TicksBothSides);
    sliderControl.setFixedSize(500, 50);
    sliderControl.setTickInterval(2);
}

SimpleSliderDialog::~SimpleSliderDialog() {
    
}