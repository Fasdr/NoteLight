#pragma once
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QAction>
#include <QMouseEvent>
#include <QPixmap>
#include <QSettings>


class WritingArea : public QWidget{
    Q_OBJECT
    public:
        WritingArea(QWidget* parent = nullptr);
        ~WritingArea();

    protected:
        // void mousePressEvent(QMouseEvent *event) override;
        void tabletEvent(QTabletEvent *event) override;
        void paintEvent(QPaintEvent *event) override;

    private:
        QPixmap canvasImage;

};

class FileActions : public QObject {
    Q_OBJECT
    public:
        QAction actionNewFile = QAction(tr("&New file"), nullptr);
        QAction actionOpenFile = QAction(tr("&Open file"), nullptr);
        QAction actionSaveFile = QAction(tr("&Save file"), nullptr);

};

class SettingsActions : public QObject {
    Q_OBJECT
    public:
        QAction actionChooseFont = QAction(tr("&Font"), nullptr);
        QAction actionSetDefault = QAction(tr("&Set default"), nullptr);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:

        QSettings appSettings;
        void loadSettings();

        FileActions fileActions;
        void configureFileMenu();
        void newFile();
        void openFile();
        void saveFile();

        SettingsActions settingsActions;
        void configureSettingsMenu();
        void chooseFont();
        void setDefault();

        WritingArea writingArea;
};