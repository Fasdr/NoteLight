#include <guiInternal.h>
#include <QLineEdit>



DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            backgroundColorButton("🌆", this),
            backgroundColorDialog(15, this),
            penColorButton("🎨", this),
            penColorDialog(0, this) {

    menuLayout.addLayout(&colorControlsLayout);
    
    QPalette pal;
    colorControlsLayout.addWidget(&backgroundColorButton);
    pal = backgroundColorButton.palette();
    pal.setColor(QPalette::Button, backgroundColorDialog.getColor());
    backgroundColorButton.setAutoFillBackground(true);
    backgroundColorButton.setPalette(pal);

    colorControlsLayout.addWidget(&penColorButton);
    pal = penColorButton.palette();
    pal.setColor(QPalette::Button, penColorDialog.getColor());
    penColorButton.setAutoFillBackground(true);
    penColorButton.setPalette(pal);

    connect(&backgroundColorButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startBackgroundColorDialog);

    connect(&backgroundColorDialog, &SimpleColorDialog::colorUpdated,
        this, &DrawingToolsMenu::updateBackgroundColorButton);

    connect(&penColorButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startPenColorDialog);

    connect(&penColorDialog, &SimpleColorDialog::colorUpdated,
        this, &DrawingToolsMenu::updatePenColorButton);
    
    menuLayout.addWidget(&zoomControl);
}

DrawingToolsMenu::~DrawingToolsMenu() {

}

void DrawingToolsMenu::startBackgroundColorDialog() {
    backgroundColorDialog.show();
}

void DrawingToolsMenu::startPenColorDialog() {
    penColorDialog.show();
}


ZoomControl* DrawingToolsMenu::getZoomControlP() {
    return &zoomControl;
}

SimpleColorDialog* DrawingToolsMenu::getBackgroundColorDialog() {
    return &backgroundColorDialog;
}

SimpleColorDialog* DrawingToolsMenu::getPenColorDialog() {
    return &penColorDialog;
}

void DrawingToolsMenu::updateBackgroundColorButton(QColor newColor) {
    QPalette pal = backgroundColorButton.palette();
    pal.setColor(QPalette::Button, newColor);
    backgroundColorButton.setAutoFillBackground(true);
    backgroundColorButton.setPalette(pal);
}

void DrawingToolsMenu::updatePenColorButton(QColor newColor) {
    QPalette pal = penColorButton.palette();
    pal.setColor(QPalette::Button, newColor);
    penColorButton.setAutoFillBackground(true);
    penColorButton.setPalette(pal);
}

