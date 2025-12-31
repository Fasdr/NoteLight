#include <QApplication>
#include <QByteArray>
#include <guiInternal.h>



namespace gui_f {
    int startApp(int argc, char *argv[]) {

        qputenv("QT_QPA_PLATFORM", "xcb");
        qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
        qputenv("QT_SCALE_FACTOR", "1");
        qputenv("QT_FONT_DPI", "96");
        

        QApplication app(argc, argv);

        QFont font{app.font()};
        app.setFont(font);

        MainWindow window;
        window.move(0, 0);
        window.setMinimumSize(200, 200);
        window.showMaximized();

        return app.exec();
    }
}