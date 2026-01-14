#include <titleBar.h>
#include <QFont>
#include <QDebug>
#include <QString>

void TitleBar::setVisuals(QMap<QString, QVariant>& parameters) {
    QFont appFont{parameters["appFont"].value<QFont>()};
    QFont iconFont{parameters["iconFont"].value<QFont>()};
    QString appTheme{parameters["appTheme"].value<QString>()};

    int exMiMaButtonSize{static_cast<int>(1.375 * iconFont.pixelSize())};

    if (fullScreenButton.text().isEmpty()) {
        fullScreenButton.setText("\ue3c2");
    }
    fullScreenButton.setFont(iconFont);
    fullScreenButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);

    if (minimizeButton.text().isEmpty()) {
        minimizeButton.setText("\ue15b");
    }
    minimizeButton.setFont(iconFont);
    minimizeButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);

    if (maximizeButton.text().isEmpty()) {
        maximizeButton.setText("\ue3c6");
    }
    maximizeButton.setFont(iconFont);
    maximizeButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);

    if (exitButton.text().isEmpty()) {
        exitButton.setText("\ue5cd");
    }
    exitButton.setFont(iconFont);
    exitButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);
    
    QString titleBarStyleSheet;
    QString fullScreenButtonStyleSheet;
    QString minimizeButtonStyleSheet;
    QString maximizeButtonStyleSheet;
    QString exitButtonStyleSheet;

    if (appTheme == "Dark") {
        titleBarStyleSheet = QString(
            "QFrame {"
                "background-color: black;"
                "border-radius: %1px;"
                "border: 4px outset #505050;"
            "}"
        ).arg(iconFont.pixelSize() / 2);

        fullScreenButtonStyleSheet = QString(
            "QPushButton {"
                "background-color: transparent;"
                "border: none;"
                "border-radius: %1px;"
                "color: white;"
                "font-size: %2px;"
                "padding-top: %3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #c7c5c6;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #676767;"
            "}"
        ).arg(exMiMaButtonSize / 2).arg(iconFont.pixelSize()).arg(static_cast<int>(iconFont.pixelSize() * 12 / 40));

        minimizeButtonStyleSheet = QString(
            "QPushButton {"
                "background-color: transparent;"
                "border: none;"
                "border-radius: %1px;"
                "color: white;"
                "font-size: %2px;"
                "padding-top: %3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #c7c5c6;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #676767;"
            "}"
        ).arg(exMiMaButtonSize / 2).arg(iconFont.pixelSize()).arg(static_cast<int>(iconFont.pixelSize() * 30 / 40));
        
        maximizeButtonStyleSheet = QString(
            "QPushButton {"
                "background-color: transparent;"
                "border: none;"
                "border-radius: %1px;"
                "color: white;"
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

        exitButtonStyleSheet = QString(
            "QPushButton {"
                "background-color: transparent;"
                "border: none;"
                "border-radius: %1px;"
                "color: white;"
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
    }

    setStyleSheet(titleBarStyleSheet);
    fullScreenButton.setStyleSheet(fullScreenButtonStyleSheet);
    minimizeButton.setStyleSheet(minimizeButtonStyleSheet);
    maximizeButton.setStyleSheet(maximizeButtonStyleSheet);
    exitButton.setStyleSheet(exitButtonStyleSheet);
}

void TitleBar::setMaximizedButton(bool isMaximized) {
    if (isMaximized) {
        maximizeButton.setText("\ue3e0");
    } else {
        maximizeButton.setText("\ue3c6");
    }
}

void TitleBar::setFullScreenButton(bool isFullScreen) {
    if (isFullScreen) {
        fullScreenButton.setText("\ue5d1");
    } else {
        fullScreenButton.setText("\ue3c2");
    }
}