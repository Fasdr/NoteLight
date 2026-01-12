#include <titleBar.h>
#include <QFont>
#include <QDebug>


void TitleBar::setVisuals(QMap<QString, QVariant>& paramters) {
    QFont appFont{paramters["appFont"].value<QFont>()};
    QFont iconFont{paramters["iconFont"].value<QFont>()};
    
    int exMiMaButtonSize{iconFont.pixelSize() + 10};

    minimizeButton.setText("\ue313");
    minimizeButton.setFont(iconFont);
    minimizeButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);
    QString minimizeButtonStyleSheet = QString(
        "QPushButton {"
            "background-color: transparent;"
            "border: none;"
            "border-radius: %1px;"
            "color: black;"
            "font-size: %2px;"
            "padding-top: %3px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c7c5c6;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #676767;"
        "}"
    ).arg(exMiMaButtonSize / 2).arg(iconFont.pixelSize()).arg(static_cast<int>(iconFont.pixelSize() * 10 / 40));
    minimizeButton.setStyleSheet(minimizeButtonStyleSheet);

    // TODO: choose Icons depending on normal/maximized window state
    maximizeButton.setText("\ue316");
    maximizeButton.setFont(iconFont);
    maximizeButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);
    QString maximizeButtonStyleSheet = QString(
        "QPushButton {"
            "background-color: transparent;"
            "border: none;"
            "border-radius: %1px;"
            "color: black;"
            "font-size: %2px;"
            "padding-top: %3px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c7c5c6;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #676767;"
        "}"
    ).arg(exMiMaButtonSize / 2).arg(iconFont.pixelSize()).arg(static_cast<int>(iconFont.pixelSize() * 10 / 40));
    maximizeButton.setStyleSheet(maximizeButtonStyleSheet);

    exitButton.setText("\ue5cd");
    exitButton.setFont(iconFont);
    exitButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);
    QString exitButtonStyleSheet = QString(
        "QPushButton {"
            "background-color: transparent;"
            "border: none;"
            "border-radius: %1px;"
            "color: black;"
            "font-size: %2px;"
            "padding-top: %3px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #f19eac;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #aa0f0f;"
        "}"
    ).arg(exMiMaButtonSize / 2).arg(iconFont.pixelSize()).arg(static_cast<int>(iconFont.pixelSize() * 12 / 40));
    exitButton.setStyleSheet(exitButtonStyleSheet);

}