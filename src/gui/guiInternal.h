#pragma once
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QAction>

class FileActions : public QObject {
    Q_OBJECT
    public:
        QAction actionNewFile = QAction(tr("&New file"), nullptr);
        QAction actionOpenFile = QAction(tr("&Open file"), nullptr);
        QAction actionSaveFile = QAction(tr("&Save file"), nullptr);

};

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:
        void newFile();
        void openFile();
        void saveFile();

        FileActions fileActions;
        void configureFileMenu();
};