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

        void setMaximizedButton(bool isMaximized);
        void setFullScreenButton(bool isFullScreen);


    protected:
        void mousePressEvent(QMouseEvent* event) override;
};