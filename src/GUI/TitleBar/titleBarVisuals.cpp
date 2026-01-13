#include <titleBar.h>
#include <QFont>
#include <QDebug>
#include <QString>

void TitleBar::setVisuals(QMap<QString, QVariant>& parameters) {
    QFont appFont{parameters["appFont"].value<QFont>()};
    QFont iconFont{parameters["iconFont"].value<QFont>()};
    QString appTheme{parameters["appTheme"].value<QString>()};

    int exMiMaButtonSize{static_cast<int>(1.375 * iconFont.pixelSize())};

    fullScreenButton.setText("\ue3c2");
    fullScreenButton.setFont(iconFont);
    fullScreenButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);

    minimizeButton.setText("\ue313");
    minimizeButton.setFont(iconFont);
    minimizeButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);

    // TODO: choose Icons depending on normal/maximized window state
    maximizeButton.setText("\ue316");
    maximizeButton.setFont(iconFont);
    maximizeButton.setFixedSize(exMiMaButtonSize, exMiMaButtonSize);

    exitButton.setText("\ue5cd");
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
        ).arg(exMiMaButtonSize / 2).arg(iconFont.pixelSize()).arg(static_cast<int>(iconFont.pixelSize() * 10 / 40));
        
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