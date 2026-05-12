#pragma once
#include <QFrame>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QPen>
#include <QPushButton>

class TitleBar : public QFrame {
    Q_OBJECT
public:
    TitleBar(QWidget* parent = nullptr);
    ~TitleBar();

    QHBoxLayout titleBarLayout;
    QMenuBar actionsMenuBar;
    QPushButton fullScreenButton;
    QPushButton minimizeButton;
    QPushButton maximizeButton;
    QPushButton exitButton;

    void setVisuals(QMap<QString, QVariant>& parameters);

    void setMaximizedButton(bool isMaximized);
    void setFullScreenButton(bool isFullScreen);

signals:
    void newPageSignal();
    void newPenSignal(QPen newPen, double newPropWidth);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};
