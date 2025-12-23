#include <guiInternal.h>

#include <QMenuBar>
#include <QFontDialog>

#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), writingArea(this) {
    configureFileMenu();
    configureSettingsMenu();
    setCentralWidget(&writingArea);
}

void MainWindow::configureFileMenu() {
    connect(&fileActions.actionNewFile, &QAction::triggered, this, &MainWindow::newFile);
    connect(&fileActions.actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(&fileActions.actionSaveFile, &QAction::triggered, this, &MainWindow::saveFile);

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addActions({
        &fileActions.actionNewFile,
        &fileActions.actionOpenFile,
        &fileActions.actionSaveFile});

    // auto extraStuff = menuBar()->addMenu(tr("&Extra"));
    // QAction* actionExtra = new QAction(tr("&Extra"), this);
    // extraStuff->addAction(actionExtra);
}

void MainWindow::configureSettingsMenu() {
    connect(&settingsActions.actionChooseFont, &QAction::triggered, this, &MainWindow::chooseFont);

    auto settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addActions({
        &settingsActions.actionChooseFont});
}

MainWindow::~MainWindow() {

}

void MainWindow::newFile() {
    std::cout << "TODO: new file" << std::endl;
}

void MainWindow::openFile() {
    std::cout << "TODO: open file" << std::endl;
}

void MainWindow::saveFile() {
    std::cout << "TODO: save file" << std::endl;
}

void MainWindow::chooseFont() {
    // QFont currentFont = qApp->font();
    // currentFont.setPointSize(28);
    // qApp->setFont(currentFont);
    bool updated = false;
    QFont updatedFont = QFontDialog::getFont(&updated, qApp->font());
    if (updated) {
        qApp->setFont(updatedFont);
    }
}
