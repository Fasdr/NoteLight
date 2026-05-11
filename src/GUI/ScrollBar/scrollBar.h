#pragma once
#include <QScrollBar>

class ScrollBar : public QScrollBar {
    Q_OBJECT
public:
    ScrollBar(QWidget* parent = nullptr);
    ~ScrollBar();

    void setVisuals(QMap<QString, QVariant>& parameters);
};
