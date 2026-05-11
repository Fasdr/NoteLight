#pragma once
#include <QScrollBar>

class ScrollBar : public QScrollBar {
    Q_OBJECT
public:
    ScrollBar(QWidget* parent = nullptr);
    ~ScrollBar();

    void setVisuals(QMap<QString, QVariant>& parameters);

    void updateScrollBar(int min, int max, int pageSize);
};
