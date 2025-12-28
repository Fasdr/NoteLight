#include <guiInternal.h>

PositionControl::PositionControl(QWidget* parent) : QWidget(parent),
        gridLayout(this) {
    std::array<QString, 9> directionElements = {
        "↖️", "⬆️", "↗️",
        "⬅️", "🔃", "➡️",
        "↙️", "⬇️", "↘️"};
    for (int i{0}; i < 9; ++i) {
        directionLabels[i].setText(directionElements[i]);
        directionLabels[i].setFixedSize(80, 80);
        directionLabels[i].setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        directionLabels[i].setAlignment(Qt::AlignCenter);
        gridLayout.addWidget(&directionLabels[i], i / 3, i % 3);
    }
    for (int i{0}; i < 9; ++i) {
        gridLayout.setColumnStretch(i, 1);
        gridLayout.setRowStretch(i, 1);
    }

}

PositionControl::~PositionControl() {

}