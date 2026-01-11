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
        QPushButton exitButton;

    protected:
        void mousePressEvent(QMouseEvent* event) override;
};