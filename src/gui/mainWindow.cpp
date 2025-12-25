#include <guiInternal.h>

#include <QMenuBar>

#include <QFontDialog>
#include <QMessageBox>

#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
        appSettings("./appSettings.ini", QSettings::IniFormat),
        writingArea(this) {
    
    loadSettings();
    configureFileMenu();
    configureSettingsMenu();
    setCentralWidget(&writingArea);
}

void MainWindow::loadSettings() {
    QFont currentFont;
    if (appSettings.contains("Font")) {
        currentFont = appSettings.value("Font").value<QFont>();
    } else {
        currentFont = QFont("FreeSans", 20, QFont::Normal, false);
        appSettings.setValue("Font", currentFont);
    }
    qApp->setFont(currentFont);
    applyFontToDrawingTools();
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
    connect(&settingsActions.actionSetDefault, &QAction::triggered, this, &MainWindow::setDefault);

    auto settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addActions({
        &settingsActions.actionChooseFont});
    settingsMenu->addSeparator();
    settingsMenu->addActions({
        &settingsActions.actionSetDefault});
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
    bool updated{false};
    QFont updatedFont = QFontDialog::getFont(&updated, qApp->font(), this);
    appSettings.setValue("Font", updatedFont);
    if (updated) {
        qApp->setFont(updatedFont);
        applyFontToDrawingTools();
    }
}

void MainWindow::applyFontToDrawingTools() {
    QFont curFont = qApp->font();
    curFont.setPointSize(curFont.pointSize() * 1.5);
    writingArea.setFont(curFont);
}

void MainWindow::setDefault() {
    QMessageBox defaultConfirmation;
    defaultConfirmation.setText("This operation will revert all application settings to defult. Are you sure that you want to proceed?");
    defaultConfirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    defaultConfirmation.setDefaultButton(QMessageBox::No);

    int response = defaultConfirmation.exec();
    if (response == QMessageBox::Yes) {
        appSettings.clear();
        loadSettings();
    }
}
