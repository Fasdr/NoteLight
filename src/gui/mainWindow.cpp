#include <guiInternal.h>

#include <QMenuBar>
#include <QAction>

#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    
    auto testAction = new QAction(tr("&Print"), this);
    connect(testAction, &QAction::triggered, this, &MainWindow::testConsole);
    auto testAction1 = new QAction(tr("&Print1"), this);
    connect(testAction1, &QAction::triggered, this, &MainWindow::testConsole);

    auto fileMenu = menuBar()->addMenu(tr("&Test"));
    fileMenu->addAction(testAction);
    fileMenu->addAction(testAction1);

}

MainWindow::~MainWindow() {

}

void MainWindow::testConsole() {
    std::cout << "Hi!" << std::endl;
}