#pragma once
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>

class TitleBar : public QFrame {
    Q_OBJECT
    public:
        TitleBar(QWidget* parent = nullptr);
        ~TitleBar();

        QHBoxLayout titleBarLayout;
        QPushButton minimizeButton;
        QPushButton maximizeButton;
        QPushButton exitButton;

        void setVisuals(QMap<QString, QVariant>& paramters);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
};