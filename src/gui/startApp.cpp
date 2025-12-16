#include <QApplication>
#include <guiInternal.h>

namespace gui_f {
    int startApp(int argc, char *argv[]) {
        QApplication app(argc, argv);
        MainWindow window;
        window.showMaximized();
        return app.exec();
    }
}