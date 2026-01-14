#pragma once
#include <QWidget>

class InputArea : public QWidget {
    Q_OBJECT

    public:
        InputArea(QWidget* parent = nullptr);
        ~InputArea();

        void setVisuals(QMap<QString, QVariant>& parameters);

    protected:
        bool event(QEvent* event) override;
};