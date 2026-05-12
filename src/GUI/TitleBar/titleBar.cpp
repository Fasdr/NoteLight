#include <titleBar.h>

#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QWindow>

namespace {
    QPen currentStrokePen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap,
                          Qt::RoundJoin);
    // StrokeType currentStrokeType{StrokeType::PolyLine};
    double currentStrokePropWidth{0.005};

    void setupPenMenu(QMenu& penMenu, TitleBar* titleBar) {
        static QMenu colorMenu{"Color"};
        penMenu.addMenu(&colorMenu);
        static QAction colorBlack{"Black"};
        colorMenu.addAction(&colorBlack);
        titleBar->connect(&colorBlack, &QAction::triggered, titleBar,
                          [titleBar]() {
                              currentStrokePen.setColor(Qt::black);
                              emit(titleBar->newPenSignal(
                                  currentStrokePen, currentStrokePropWidth));
                          });
        static QAction colorBlue{"Blue"};
        colorMenu.addAction(&colorBlue);
        titleBar->connect(&colorBlue, &QAction::triggered, titleBar,
                          [titleBar]() {
                              currentStrokePen.setColor(Qt::blue);
                              emit(titleBar->newPenSignal(
                                  currentStrokePen, currentStrokePropWidth));
                          });
        static QAction colorRed{"Red"};
        colorMenu.addAction(&colorRed);
        titleBar->connect(&colorRed, &QAction::triggered, titleBar,
                          [titleBar]() {
                              currentStrokePen.setColor(Qt::red);
                              emit(titleBar->newPenSignal(
                                  currentStrokePen, currentStrokePropWidth));
                          });
    }

    void setupPageMenu(QMenu& pageMenu, TitleBar* titleBar) {
        static QAction newPageAction{"New Page"};
        pageMenu.addAction(&newPageAction);
        titleBar->connect(&newPageAction, &QAction::triggered, titleBar,
                          [titleBar]() { emit(titleBar->newPageSignal()); });
    }
}

TitleBar::TitleBar(QWidget* parent) : QFrame(parent), titleBarLayout(this) {
    titleBarLayout.setContentsMargins(5, 0, 5, 0);
    titleBarLayout.setAlignment(Qt::AlignVCenter);

    titleBarLayout.addWidget(&actionsMenuBar, 0, Qt::AlignVCenter);
    static QMenu penMenu{"Pen"};
    setupPenMenu(penMenu, this);
    actionsMenuBar.addMenu(&penMenu);
    static QMenu pageMenu{"Page"};
    setupPageMenu(pageMenu, this);
    actionsMenuBar.addMenu(&pageMenu);

    titleBarLayout.addStretch();

    titleBarLayout.addWidget(&fullScreenButton);
    titleBarLayout.addWidget(&minimizeButton);
    titleBarLayout.addWidget(&maximizeButton);
    titleBarLayout.addWidget(&exitButton);
}

TitleBar::~TitleBar() {}

void TitleBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QWindow* window = this->window()->windowHandle();
        if (window) {
            bool success = window->startSystemMove();
        }
        event->accept();
    }
}
