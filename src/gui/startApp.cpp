#include <QApplication>
#include <QByteArray>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>
#include <guiInternal.h>

const QByteArray QML_LOADER = R"(
    import QtQuick 2.0
    import QtQuick.VirtualKeyboard 2.1

    // This component is the official Qt Virtual Keyboard
    InputPanel {
        id: inputPanel
        z: 99
        
        // Optional: Hardcode the size or bindings here
        width: 800
        height: 300
        
        // Connection to show/hide the window based on active focus
        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges { target: inputPanel; y: 0 } // Animation logic could go here
        }
    }
)";

namespace gui_f {
    int startApp(int argc, char *argv[]) {

        qputenv("QT_QPA_PLATFORM", "xcb");
        qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
        qputenv("QT_SCALE_FACTOR", "1");
        qputenv("QT_FONT_DPI", "96");
        
        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
        qputenv("QT_VIRTUALKEYBOARD_STYLE", QByteArray("QtQuick"));


        QApplication app(argc, argv);

        QFont font{app.font()};
        app.setFont(font);

        MainWindow window;
        window.move(0, 0);
        window.setMinimumSize(200, 200);
        window.showMaximized();

        
        QQuickView keyboardView;
        keyboardView.setFlags(Qt::FramelessWindowHint | 
                          Qt::Tool | 
                          Qt::WindowStaysOnTopHint | 
                          Qt::WindowDoesNotAcceptFocus);

        QQmlComponent component(keyboardView.engine());
        component.setData(QML_LOADER, QUrl("keyboard_loader.qml"));
        
        QObject *object = component.create();
        if (object) {
            QQuickItem *item = qobject_cast<QQuickItem*>(object);
            item->setParentItem(keyboardView.contentItem());
            QObject::connect(item, SIGNAL(activeChanged()), &keyboardView, SLOT(show()));
        }

        keyboardView.resize(800, 300);
        keyboardView.setPosition(0, 500);

        return app.exec();
    }
}