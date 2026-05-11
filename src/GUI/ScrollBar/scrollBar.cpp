#include <scrollBar.h>

ScrollBar::ScrollBar(QWidget* parent) : QScrollBar(parent) {};

ScrollBar::~ScrollBar() {}

void ScrollBar::updateScrollBar(int min, int max, int pageSize) {
    setMinimum(min);
    setMaximum(max);
    setPageStep(pageSize);
}
