#include <mainWindow.h>

void MainWindow::setVisuals(QMap<QString, QVariant>& paramters) {
    titleBar.setVisuals(paramters);
    inputArea.setVisuals(paramters);
}