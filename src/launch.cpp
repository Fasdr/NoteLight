#include <launch.h>
#include <mainWindow.h>

#include <QApplication>

int launch(int argc, char *argv[]) {
    qputenv("QT_QPA_PLATFORM", "xcb");
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
    qputenv("QT_SCALE_FACTOR", "1");
    qputenv("QT_FONT_DPI", "96");
    
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication app(argc, argv);

    MainWindow window;
    
    window.setMinimumSize(400, 400);
    window.show();

    return app.exec();
}