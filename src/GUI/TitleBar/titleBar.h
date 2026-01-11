#pragma once
#include <QFrame>
#include <QPointF>


class TitleBar : public QFrame {
    Q_OBJECT
    public:
        TitleBar(QWidget* parent = nullptr);
        ~TitleBar();

    signals:
        void sendStartDraggingPosition(QPointF startPosition);
        void sendNewDraggingPosition(QPointF newPosition);

    protected:
        void mouseMoveEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
};