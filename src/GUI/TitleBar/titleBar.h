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
        QPushButton fullScreenButton;
        QPushButton minimizeButton;
        QPushButton maximizeButton;
        QPushButton exitButton;

        void setVisuals(QMap<QString, QVariant>& parameters);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
};