#include <guiInternal.h>
#include <QLineEdit>



DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            changeFullScreen("⛶", this),
            backgroundColorButton("🌆", this),
            backgroundColorDialog(15, this),
            penColorButton("🎨", this),
            penColorDialog(0, this),
            penWidthButton("✒️", this) {

    menuLayout.addWidget(&changeFullScreen);

    menuLayout.addLayout(&drawingControlsLayout);
    
    QPalette pal;
    drawingControlsLayout.addWidget(&backgroundColorButton);
    pal = backgroundColorButton.palette();
    pal.setColor(QPalette::Button, backgroundColorDialog.getColor());
    backgroundColorButton.setAutoFillBackground(true);
    backgroundColorButton.setPalette(pal);

    drawingControlsLayout.addWidget(&penColorButton);
    pal = penColorButton.palette();
    pal.setColor(QPalette::Button, penColorDialog.getColor());
    penColorButton.setAutoFillBackground(true);
    penColorButton.setPalette(pal);

    drawingControlsLayout.addWidget(&penWidthButton);

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

