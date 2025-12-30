#include <guiInternal.h>

#include <QMenuBar>

#include <QFileDialog>
#include <QFile>
#include <QFontDialog>
#include <QMessageBox>

#include <iostream>
#include <chrono>
#include <iostream>
#include <thread>
#include <format>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
        appSettings("./appSettings.ini", QSettings::IniFormat),
        writingArea(this),
        appSession("./appSession.ini", QSettings::IniFormat) {
    
    loadSettings();
    configureFileMenu();
    configureSettingsMenu();

    connect(writingArea.getDrawingToolsMenu()->getChangeFullScreenButton(), &QPushButton::clicked,
            this, &MainWindow::changeFullScreen);
    
    
    setCentralWidget(&writingArea);

    loadSession();

    connect(&writingArea, &WritingArea::changeMade,
            this, &MainWindow::configureTitle);
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
    connect(&fileActions.actionSaveFileAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(&fileActions.actionExitApp, &QAction::triggered, this, &MainWindow::exitApp);

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addActions({
        &fileActions.actionNewFile,
        &fileActions.actionOpenFile,
        &fileActions.actionSaveFile,
        &fileActions.actionSaveFileAs});
    fileMenu->addSeparator();
    fileMenu->addActions({
        &fileActions.actionExitApp});
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
    if (suggestToSaveOrAbort()) {
        return;
    }
    QMap<qint32, QList<LineSegment>> data;
    writingArea.setQInternalStore(std::move(data));
    currentWorkingFile = "";
    configureTitle();
}

void MainWindow::openFile() {
    if (suggestToSaveOrAbort()) {
        return;
    }
    QString fileName;
    QFileDialog openFileDialog(this, "Open File");
    openFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    const QStringList filters({"Note Light Canvas file (*.nlca)"});
    openFileDialog.setNameFilters(filters);
    openFileDialog.exec();
    if (openFileDialog.selectedFiles().empty()) {
        std::cout << "Nothing to open" << std::endl;
        return;
    }
    fileName = openFileDialog.selectedFiles()[0];
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    QDataStream input(&file);
    quint32 fileVersionBig, fileVersionSmall;
    input >> fileVersionBig >> fileVersionSmall;
    if (fileVersionBig != 0 || fileVersionSmall != 5) {
        std::cout << "File version is "<< fileVersionBig << "." << fileVersionSmall << std::endl;
        QMessageBox::warning(this, "Warning", "This file version is not supported!");
        return;
    }
    input.setVersion(QDataStream::Qt_6_10);
    QMap<qint32, QList<LineSegment>> data;
    input >> data;
    writingArea.setQInternalStore(std::move(data));
    currentWorkingFile = fileName;
    configureTitle();
}

void MainWindow::saveFile() {
    if (currentWorkingFile.isEmpty()) {
        saveFileAs();
        return;
    }
    QFile file(currentWorkingFile);
    if (!file.open(QIODevice::WriteOnly)) {
        file.close();
        saveFileAs();
        return;
    }
    // Writing into a file
    QDataStream output(&file);
    output << versionBig << versionSmall;
    output.setVersion(QDataStream::Qt_6_10);
    output << writingArea.getQInternalStore();
    file.close();
    writingArea.setHasUnsavedChanges(false);
    configureTitle();
}

void MainWindow::saveFileAs() {
    QString fileName;
    // fileName = QFileDialog::getSaveFileName(this, "Save As");
    QFileDialog saveFileDialog(this, "Save File As");
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    const QStringList filters({"Note Light Canvas file (*.nlca)"});
    saveFileDialog.setNameFilters(filters);
    saveFileDialog.exec();
    // saveFileDialog.set
    if (saveFileDialog.selectedFiles().empty()) {
        std::cout << "No file to save into!" << std::endl;
        return;
    }
    fileName = saveFileDialog.selectedFiles()[0];
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    // Writing into a file
    QDataStream output(&file);
    output << versionBig << versionSmall;
    output.setVersion(QDataStream::Qt_6_10);
    output << writingArea.getQInternalStore();
    file.close();
    writingArea.setHasUnsavedChanges(false);
    currentWorkingFile = fileName;
    configureTitle();
}

void MainWindow::loadSession() {
    if (appSession.contains("CurrentWorkingFile")) {
        currentWorkingFile = appSession.value("CurrentWorkingFile").value<QString>();
    } else {
        currentWorkingFile = "";
    }
    if (currentWorkingFile.isEmpty()) {
        configureTitle();
        return;
    }
    QFile file(currentWorkingFile);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << "Loading... Cannot open file: " << file.errorString().toStdString() << std::endl;
        currentWorkingFile = "";
        configureTitle();
        return;
    }

    QDataStream input(&file);
    quint32 fileVersionBig, fileVersionSmall;
    input >> fileVersionBig >> fileVersionSmall;

    if (fileVersionBig != 0 || fileVersionSmall != 5) {
        std::cout << "File version is "<< fileVersionBig << "." << fileVersionSmall << std::endl;
        QMessageBox::warning(this, "Warning", "This file version is not supported!");
        currentWorkingFile = "";
        configureTitle();
        return;
    }
    input.setVersion(QDataStream::Qt_6_10);
    QMap<qint32, QList<LineSegment>> data;
    input >> data;
    writingArea.setQInternalStore(std::move(data));
    configureTitle();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (suggestToSaveOrAbort()) {
        return;
    }
    event->accept();
}

void MainWindow::exitApp() {
    if (suggestToSaveOrAbort()) {
        return;
    }
    appSession.setValue("CurrentWorkingFile", currentWorkingFile);
    qApp->exit();
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

bool MainWindow::suggestToSaveOrAbort() {
    QMessageBox saveQuestion;
    saveQuestion.setText("This file has unsaved changes. Do you want to save them first?");
    saveQuestion.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    saveQuestion.setDefaultButton(QMessageBox::Yes);

    int response{QMessageBox::Yes};
    while (writingArea.getHasUnsavedChanges() && response == QMessageBox::Yes) {
        response = saveQuestion.exec();
        if (response == QMessageBox::Yes) {
            saveFile();
        }
    }
    return !(response == QMessageBox::No || response == QMessageBox::Yes);
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

void MainWindow::changeFullScreen() {
    if (isFullScreen()) {
        setMinimumSize(QGuiApplication::primaryScreen()->availableGeometry().size().width(),
                        QGuiApplication::primaryScreen()->availableGeometry().size().height() * 0.975);
        move(0, 0);
        setWindowState(Qt::WindowMaximized);
    } else {
        setMinimumSize(QGuiApplication::primaryScreen()->availableGeometry().size().width(),
                        QGuiApplication::primaryScreen()->availableGeometry().size().height() * 0.975);
        // setMinimumSize(QGuiApplication::primaryScreen()->availableGeometry().size());
        move(0, 0);
        setWindowState(Qt::WindowFullScreen);
    }
}

void MainWindow::configureTitle() {
    QString currentTitle = QString("Note Light - %1 %2")
        .arg(currentWorkingFile.isEmpty() ? "Untitled" : currentWorkingFile)
        .arg(writingArea.getHasUnsavedChanges() ? "*" : "");
    setWindowTitle(currentTitle);
}
