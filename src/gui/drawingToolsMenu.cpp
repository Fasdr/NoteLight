#include <guiInternal.h>
#include <QLineEdit>



DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            backgroundColorButton("🌆", this),
            backgroundColorDialog(this),
            zoomControl(this) {

    menuLayout.addWidget(&backgroundColorButton);
    QPalette pal = backgroundColorButton.palette();
    pal.setColor(QPalette::Button, backgroundColorDialog.getColor());
    backgroundColorButton.setAutoFillBackground(true);
    backgroundColorButton.setPalette(pal);

    connect(&backgroundColorButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startBackgroundColorDialog);

    connect(&backgroundColorDialog, &SimpleColorDialog::colorUpdated,
        this, &DrawingToolsMenu::updateBackgroundColorButton);
    
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

SimpleColorDialog* DrawingToolsMenu::getBackgroundColorDialog() {
    return &backgroundColorDialog;
}

void DrawingToolsMenu::updateBackgroundColorButton(QColor newColor) {
    QPalette pal = backgroundColorButton.palette();
    pal.setColor(QPalette::Button, newColor);
    backgroundColorButton.setAutoFillBackground(true);
    backgroundColorButton.setPalette(pal);
}

