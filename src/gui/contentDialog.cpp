#include <guiInternal.h>
#include <QFileInfo>

#include <string>

ContentDialog::ContentDialog(QWidget* parent) : QDialog(parent),
            contentDialogLayout(this),
            currentRoot(&contentTree),
            createItemButton("➕", this),
            removeItemButton("➖", this) {
    contentDialogLayout.addWidget(&contentTree);
    contentDialogLayout.addLayout(&entryLayout);
    entryLayout.addWidget(&newItemLineEdit);
    entryLayout.addWidget(&createItemButton);
    entryLayout.addWidget(&removeItemButton);

    contentTree.setColumnCount(1);
    contentTree.setHeaderLabel("Title/Chapter/Topic");
    
    contentTree.setIndentation(75);
    
    setTreeSize();
    
    contentTree.setAcceptDrops(true);
    contentTree.setDragEnabled(true);
    contentTree.setDragDropMode(QAbstractItemView::InternalMove);
    contentTree.setDragDropOverwriteMode(false);

    currentRoot.setText(0, "Test");
    double xOriginDefault{}, yOriginDefault{}, zoomDefault{1.0};
    currentRoot.setData(1, Qt::UserRole, xOriginDefault);
    currentRoot.setData(2, Qt::UserRole, yOriginDefault);
    currentRoot.setData(3, Qt::UserRole, zoomDefault);

    currentRoot.setFlags(currentRoot.flags() & ~Qt::ItemIsDragEnabled);

    // for (auto i{0}; i < 10; ++i) {
    //     QTreeWidgetItem* top = new QTreeWidgetItem(&contentTree);
    //     top->setText(0, QString("Test%1").arg(i));
    //     QTreeWidgetItem* bottom = new QTreeWidgetItem(top);
    //     bottom->setText(0, QString("Test%1").arg(10 * i));
    // }
    
    connect(&createItemButton, &QPushButton::clicked,
            this, &ContentDialog::addNewItem);

    connect(&removeItemButton, &QPushButton::clicked,
            this, &ContentDialog::removeSelectedItem);

    
}

ContentDialog::~ContentDialog() {

}

void ContentDialog::addNewItem() {
    QString entryName{newItemLineEdit.text().simplified()};
    if (entryName.isEmpty()) {
        return;
    }
    QTreeWidgetItem* top = new QTreeWidgetItem(&currentRoot);
    top->setText(0, entryName);
    emit positionItemCreated(top);
    currentRoot.setExpanded(true);
}

void ContentDialog::removeSelectedItem() {
    QList<QTreeWidgetItem*> selectedContentItems{contentTree.selectedItems()};
    if (selectedContentItems.size() == 0) {
        return;
    }
    QTreeWidgetItem* itemToRemove{selectedContentItems[0]};
    if (itemToRemove->parent() == nullptr) {
        return;
    } else {
        itemToRemove->parent()->removeChild(itemToRemove);
    }
    delete itemToRemove;
}

void ContentDialog::updatedRootName(QString currentWorkingFile) {
    if (currentWorkingFile.isEmpty()) {
        currentRoot.setText(0, "Untitled");
    } else {
        currentRoot.setText(0, QFileInfo(currentWorkingFile).fileName());
    }
}

void ContentDialog::setTreeSize() {
    contentTree.setMinimumSize(QGuiApplication::primaryScreen()->geometry().width() / 4,
                               QGuiApplication::primaryScreen()->geometry().height() / 2);
}

QTreeWidget* ContentDialog::getContentTree() {
    return &contentTree;
}