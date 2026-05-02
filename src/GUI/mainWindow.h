#pragma once
#include <inputArea.h>
#include <titleBar.h>

#include <QMap>
#include <QVariant>
#include <QWidget>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    TitleBar titleBar;
    InputArea inputArea;
    void setVisuals(QMap<QString, QVariant>& paramters);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;

    void changeEvent(QEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

    void closeEvent(QCloseEvent* event) override;

private:
    void exitApp();
};
