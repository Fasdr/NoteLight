#include <titleBar.h>

#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QWindow>

TitleBar::TitleBar(QWidget* parent) : QFrame(parent), titleBarLayout(this) {
    titleBarLayout.setContentsMargins(5, 0, 5, 0);
    titleBarLayout.setAlignment(Qt::AlignVCenter);

    titleBarLayout.addWidget(&actionsMenuBar, 0, Qt::AlignVCenter);
    static QMenu pageMenu{"Page"};
    static QAction newPageAction{"NewPage"};
    pageMenu.addAction(&newPageAction);
    connect(&newPageAction, &QAction::triggered, this,
            [this]() { emit(newPageSignal()); });
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
