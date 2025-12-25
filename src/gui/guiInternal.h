#pragma once
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QAction>
#include <QMouseEvent>
#include <QPixmap>
#include <QSettings>
#include <QPen>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>

#include <unordered_map>
#include <utility>

class DrawingToolsMenu : public QWidget {
    Q_OBJECT
    public:
        DrawingToolsMenu(QWidget* parent = nullptr);
        ~DrawingToolsMenu();

    signals:
        void requireBackgroundColor();

    private:
        QVBoxLayout menuLayout;
        QPushButton backgroundColorButton;
        QSpinBox zoomSpinBox;
};

struct LineSegment {
    QPointF start, end;
    QColor color;
    double width;
};

class WritingArea : public QWidget{
    Q_OBJECT
    public:
        WritingArea(QWidget* parent = nullptr);
        ~WritingArea();

    protected:
        // void mousePressEvent(QMouseEvent *event) override;
        void tabletEvent(QTabletEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    public slots:
        void updateBackgroundColor();
        void updateZoom(int newZoomValue);

    private:
        DrawingToolsMenu drawingToolsMenu;
        std::pair<int, int> getCoordinates(QPointF point);
        QPixmap canvasImage;
        double xOrigin{}, yOrigin{}, zoom{1.0};
        int patchSize;
        int nPatches{2000}; // is not consistent with different screens, will be fixed later
        std::unordered_map<int, std::vector<LineSegment>> internalStore;
        int combineIntoIndex(int i, int j);
        QPointF lastPoint;
        QPen canvasPen;
        QColor canvasBackgroundColor{0, 0, 0};

        int processSegment(QPointF startPoint, QPointF finishPoint);
        int recreateCanvas();

        void updateDrawingToolsMenuPosition();
};

class FileActions : public QObject {
    Q_OBJECT
    public:
        QAction actionNewFile = QAction(tr("&New file"), nullptr);
        QAction actionOpenFile = QAction(tr("&Open file"), nullptr);
        QAction actionSaveFile = QAction(tr("&Save file"), nullptr);

};

class SettingsActions : public QObject {
    Q_OBJECT
    public:
        QAction actionChooseFont = QAction(tr("&Font"), nullptr);
        QAction actionSetDefault = QAction(tr("&Set default"), nullptr);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:

        QSettings appSettings;
        void loadSettings();

        FileActions fileActions;
        void configureFileMenu();
        void newFile();
        void openFile();
        void saveFile();

        SettingsActions settingsActions;
        void configureSettingsMenu();
        void chooseFont();
        void setDefault();

        WritingArea writingArea;
};