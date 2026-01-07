#include <guiInternal.h>
#include <string>

ContentDialog::ContentDialog(QWidget* parent) : QDialog(parent),
            conentDialogLayout(this) {
    conentDialogLayout.addWidget(&contentTree);
    contentTree.setColumnCount(1);
    contentTree.setHeaderLabel("Title");
    
    contentTree.setIndentation(75);
    
    setTreeSize();
    
    contentTree.setAcceptDrops(true);
    contentTree.setDragEnabled(true);
    contentTree.setDragDropMode(QAbstractItemView::InternalMove);
    contentTree.setDragDropOverwriteMode(false);

    for (auto i{0}; i < 10; ++i) {
        QTreeWidgetItem* top = new QTreeWidgetItem(&contentTree);
        top->setText(0, QString("Test%1").arg(i));
        QTreeWidgetItem* bottom = new QTreeWidgetItem(top);
        bottom->setText(0, QString("Test%1").arg(10 * i));
    }
    
}

ContentDialog::~ContentDialog() {

}

void ContentDialog::setTreeSize() {
    contentTree.setMinimumSize(QGuiApplication::primaryScreen()->geometry().width() / 4,
                               QGuiApplication::primaryScreen()->geometry().height() / 2);

}