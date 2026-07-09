#pragma once
#include <QPen>
#include <QPixmap>
#include <QWidget>
class InputArea : public QWidget {
    Q_OBJECT

public:
    InputArea(QWidget* parent = nullptr);
    ~InputArea();

    void setVisuals(QMap<QString, QVariant>& parameters);
    QPixmap preparePage(int pageNumber);
    void setScrollBar();
    void getScroll(int val);
    void newPage();
    void setNewPen(QPen newPen, double newPropWidth);
    void undoLast();
    void redoLast();

signals:
    void signalScrollBar(int min, int max, int pageSize);

protected:
    void tabletEvent(QTabletEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};
