#include <guiInternal.h>
#include <QLineEdit>



DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            backgroundColorButton("B Col", this),
            zoomControl(this) {
    // setStyleSheet("background-color: rgba(50, 50, 50, 200); border: 4px solid white; border-radius: 5px;");
    // QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(new QPushButton("Tool A"));
    // layout->addWidget(new QPushButton("Tool B"));
    // setLayout(layout);

    connect(&backgroundColorButton, &QPushButton::clicked,
        this, [this]() { emit requireBackgroundColor(); });
    menuLayout.addWidget(&backgroundColorButton);
    
    menuLayout.addWidget(&zoomControl);
}

DrawingToolsMenu::~DrawingToolsMenu() {

}

ZoomControl* DrawingToolsMenu::getZoomControlP() {
    return &zoomControl;
}

