#include <guiInternal.h>
#include <QLineEdit>



DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            backgroundColorButton("B Col", this),
            backgroundColorDialog(this),
            zoomControl(this) {

    // connect(&backgroundColorButton, &QPushButton::clicked,
    //     this, [this]() { emit requireBackgroundColor(); });

    

    menuLayout.addWidget(&backgroundColorButton);

    connect(&backgroundColorButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startBackgroundColorDialog);
    
    menuLayout.addWidget(&zoomControl);
}

DrawingToolsMenu::~DrawingToolsMenu() {

}


void DrawingToolsMenu::startBackgroundColorDialog() {
    backgroundColorDialog.show();
}

ZoomControl* DrawingToolsMenu::getZoomControlP() {
    return &zoomControl;
}

