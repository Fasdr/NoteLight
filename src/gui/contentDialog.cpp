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
    emit positionItemRemoved();
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

int collectChildrenViaDFS(QList<qreal>& xOriginVals, QList<qreal>& yOriginVals, QList<qreal>& zoomVals, QList<QString>& textVals,
                          QList<QPair<qsizetype, qsizetype>>& parentChildArr, QTreeWidgetItem* curNode, int curCount) {
    xOriginVals.push_back(curNode->data(1, Qt::UserRole).toFloat());
    yOriginVals.push_back(curNode->data(2, Qt::UserRole).toFloat());
    zoomVals.push_back(curNode->data(3, Qt::UserRole).toFloat());
    textVals.push_back(curNode->text(0));
    int curNodeCount{curCount};
    int nChildren{curNode->childCount()};
    for (int i{0}; i < nChildren; ++i) {
        QTreeWidgetItem* nextNode{curNode->child(i)};
        parentChildArr.push_back({curNodeCount, curCount + 1});
        curCount = collectChildrenViaDFS(xOriginVals, yOriginVals, zoomVals, textVals, parentChildArr, nextNode, curCount + 1);
    }
    return curCount;
}

void ContentDialog::serializeContentTree(QDataStream& output) {
    QList<qreal> xOriginVals, yOriginVals, zoomVals;
    QList<QString> textVals;
    QList<QPair<qsizetype, qsizetype>> parentChildArr;
    collectChildrenViaDFS(xOriginVals, yOriginVals, zoomVals, textVals, parentChildArr, &currentRoot, 0);
    output << xOriginVals << yOriginVals << zoomVals;
    output << textVals;
    output << parentChildArr;
}

void ContentDialog::deserializeContentTree(QDataStream& input) {
    for (QTreeWidgetItem* child : currentRoot.takeChildren()) {
        delete child;
    }
    QList<qreal> xOriginVals, yOriginVals, zoomVals;
    input >> xOriginVals >> yOriginVals >> zoomVals;
    QList<QString> textVals;
    input >> textVals;
    QList<QPair<qsizetype, qsizetype>> parentChildArr;
    input >> parentChildArr;
    qsizetype n{xOriginVals.size()};

    QTreeWidgetItem** entries = new QTreeWidgetItem*[n];
    entries[0] = &currentRoot;
    for (qsizetype i{1}; i < n; ++i) {
        entries[i] = new QTreeWidgetItem();
        entries[i]->setText(0, textVals[i]);
        entries[i]->setData(1, Qt::UserRole, xOriginVals[i]);
        entries[i]->setData(2, Qt::UserRole, yOriginVals[i]);
        entries[i]->setData(3, Qt::UserRole, zoomVals[i]);
    }
    for (auto [parent, child] : parentChildArr) {
        entries[parent]->addChild(entries[child]);
    }
    delete[] entries;

    currentRoot.setExpanded(true);
}