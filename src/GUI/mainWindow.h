#pragma once
#include <titleBar.h>

#include <QWidget>
#include <QPointF>

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        TitleBar titleBar;

    public slots:
        void acceptStartDraggingPosition(QPointF startDraggingPosition);
        void acceptNewDraggingPosition(QPointF newDraggingPosition);

    protected:
        void resizeEvent(QResizeEvent* event) override;
};