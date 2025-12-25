#include <guiInternal.h>



DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            backgroundColorButton(tr("&B Col")) {
    // setStyleSheet("background-color: rgba(50, 50, 50, 200); border: 4px solid white; border-radius: 5px;");
    // QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(new QPushButton("Tool A"));
    // layout->addWidget(new QPushButton("Tool B"));
    // setLayout(layout);
    // resize(400, 400);
    connect(&backgroundColorButton, &QPushButton::clicked,
        this, [this]() { emit requireBackgroundColor(); });
    menuLayout.addWidget(&backgroundColorButton);
}

DrawingToolsMenu::~DrawingToolsMenu() {

}

