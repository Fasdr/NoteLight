#pragma once
#include <titleBar.h>

#include <QWidget>

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        TitleBar titleBar;

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

        void resizeEvent(QResizeEvent* event) override;

        void closeEvent(QCloseEvent *event) override;

    private:
        void exitApp();
};