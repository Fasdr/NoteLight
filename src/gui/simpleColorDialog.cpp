#include <guiInternal.h>


SimpleColorDialog::SimpleColorDialog(QWidget* parent) : QDialog(parent),
    colorGrid(this) {
    for (int i{0}; i < nColors; ++i) {
        colorButtons[i].setParent(this);
        colorButtons[i].setFocusPolicy(Qt::NoFocus);
        QPalette pal = colorButtons[i].palette();
        pal.setColor(QPalette::Button, colors16[i]);
        colorButtons[i].setAutoFillBackground(true);
        colorButtons[i].setPalette(pal);
        colorGrid.addWidget(&colorButtons[i], i / nColumn, i % nColumn);

        connect(&colorButtons[i], &QPushButton::clicked,
            this, [this, i]() { 
                if (colors16[i] != pickedColor) {
                    this->setColor(colors16[i]);
                }
                this->accept(); });
    }
    
}

QColor SimpleColorDialog::getColor() {
    return pickedColor;
}

void SimpleColorDialog::setColor(QColor newColor) {
    pickedColor = newColor;
    emit colorUpdated(newColor);
}

SimpleColorDialog::~SimpleColorDialog() {

}