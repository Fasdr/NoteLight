#include <guiInternal.h>
#include <QLineEdit>

DrawingToolsMenu::DrawingToolsMenu(QWidget* parent) : QWidget(parent),
            menuLayout(this),
            changeFullScreenButton("⛶", this),
            // backgroundColorButton("🌆", this),
            backgroundColorButtonLabel("🛤️"),
            backgroundColorDialog(15, this),
            // penColorButton("🎨", this),
            penColorButtonLabel("🎨"),
            penColorDialog(0, this),
            // penWidthButton("✒️", this),
            penWidthButtonLabel("✒️"),
            penWidthDialog(this),
            zoomControl(this) {

    menuLayout.addWidget(&changeFullScreenButton);

    menuLayout.addLayout(&drawingControlsLayout);
    
    QPalette pal;

    // drawingControlsLayout.addWidget(&backgroundColorButton);
    // pal = backgroundColorButton.palette();
    // pal.setColor(QPalette::Button, backgroundColorDialog.getColor());
    // backgroundColorButton.setAutoFillBackground(true);
    // backgroundColorButton.setPalette(pal);

    drawingControlsLayout.addWidget(&backgroundColorButton);

    backgroundColorButton.setLayout(&backgroundColorButtonLayout);
    backgroundColorButtonLayout.addWidget(&backgroundColorButtonLabel);
    backgroundColorButtonLayout.setAlignment(Qt::AlignCenter);
    
    backgroundColorButtonLabel.setMargin(5);
    pal = backgroundColorButtonLabel.palette();
    pal.setColor(QPalette::Window, backgroundColorDialog.getColor());
    backgroundColorButtonLabel.setAutoFillBackground(true);
    backgroundColorButtonLabel.setBackgroundRole(QPalette::Window);
    backgroundColorButtonLabel.setPalette(pal);

    drawingControlsLayout.addWidget(&penColorButton);

    penColorButton.setLayout(&penColorButtonLayout);
    penColorButtonLayout.addWidget(&penColorButtonLabel);
    penColorButtonLayout.setAlignment(Qt::AlignCenter);
    
    penColorButtonLabel.setMargin(5);
    pal = penColorButtonLabel.palette();
    pal.setColor(QPalette::Window, penColorDialog.getColor());
    penColorButtonLabel.setAutoFillBackground(true);
    penColorButtonLabel.setBackgroundRole(QPalette::Window);
    penColorButtonLabel.setPalette(pal);

    drawingControlsLayout.addWidget(&penWidthButton);

    penWidthButton.setLayout(&penWidthButtonLayout);
    penWidthButtonLayout.addWidget(&penWidthButtonLabel);
    penWidthButtonLayout.setAlignment(Qt::AlignCenter);

    penWidthButtonLabel.setMargin(5);

    connect(&backgroundColorButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startBackgroundColorDialog);

    connect(&backgroundColorDialog, &SimpleColorDialog::colorUpdated,
        this, &DrawingToolsMenu::updateBackgroundColorButton);

    connect(&penColorButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startPenColorDialog);

    connect(&penColorDialog, &SimpleColorDialog::colorUpdated,
        this, &DrawingToolsMenu::updatePenColorButton);

    connect(&penWidthButton, &QPushButton::clicked,
        this, &DrawingToolsMenu::startPenWidthDialog);

    
    
    menuLayout.addWidget(&zoomControl);
}

DrawingToolsMenu::~DrawingToolsMenu() {

}

QPushButton* DrawingToolsMenu::getChangeFullScreenButton() {
    return &changeFullScreenButton;
}


void DrawingToolsMenu::startBackgroundColorDialog() {
    backgroundColorDialog.show();
}

void DrawingToolsMenu::startPenColorDialog() {
    penColorDialog.show();
}

void DrawingToolsMenu::startPenWidthDialog() {
    penWidthDialog.show();
}


ZoomControl* DrawingToolsMenu::getZoomControl() {
    return &zoomControl;
}

SimpleColorDialog* DrawingToolsMenu::getBackgroundColorDialog() {
    return &backgroundColorDialog;
}

SimpleColorDialog* DrawingToolsMenu::getPenColorDialog() {
    return &penColorDialog;
}

void DrawingToolsMenu::updateBackgroundColorButton(QColor newColor) {
    QPalette pal = backgroundColorButtonLabel.palette();
    pal.setColor(QPalette::Window, backgroundColorDialog.getColor());
    backgroundColorButtonLabel.setAutoFillBackground(true);
    backgroundColorButtonLabel.setBackgroundRole(QPalette::Window);
    backgroundColorButtonLabel.setPalette(pal);
}

void DrawingToolsMenu::updatePenColorButton(QColor newColor) {
    QPalette pal = penColorButtonLabel.palette();
    pal.setColor(QPalette::Window, penColorDialog.getColor());
    penColorButtonLabel.setAutoFillBackground(true);
    penColorButtonLabel.setBackgroundRole(QPalette::Window);
    penColorButtonLabel.setPalette(pal);
}

