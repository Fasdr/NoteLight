#pragma once
#include <titleBar.h>

#include <QWidget>
#include <QMap>
#include <QVariant>

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        TitleBar titleBar;
        void setVisuals(QMap<QString, QVariant>& paramters);

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

        void changeEvent(QEvent *event);

        void resizeEvent(QResizeEvent* event) override;

        void closeEvent(QCloseEvent *event) override;

    private:
        void exitApp();
};