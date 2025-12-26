#include <guiInternal.h>


SimpleColorDialog::SimpleColorDialog(QWidget* parent) : QDialog(parent),
    colorGrid(this) {
    for (int i{0}; i < nColors; ++i) {
        colorButtons[i].setParent(this);
        colorButtons[i].setFocusPolicy(Qt::NoFocus);
        colorGrid.addWidget(&colorButtons[i], i / nColumn, i % nColumn);
    }
    
}

SimpleColorDialog::~SimpleColorDialog() {

}