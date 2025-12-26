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
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QGridLayout>
#include <QFrame>

#include <array>
#include <vector>
#include <unordered_map>
#include <utility>
#include <array>

class SimpleColorDialog : public QDialog {
    Q_OBJECT
    public:
        SimpleColorDialog(int startingColor = 0, QWidget* parent = nullptr);
        ~SimpleColorDialog();
        QColor getColor();

    signals:
        void colorUpdated(QColor newColor);

    private:
        static constexpr int nColors{16};
        static constexpr int nColumn{4};
        std::array<QColor, nColors> colors16{
            QColor("#ffffff"),QColor("#fcf305"),QColor("#ff6402"),QColor("#dd0806"),
            QColor("#f20884"),QColor("#4600a5"),QColor("#0000d4"),QColor("#02abea"),
            QColor("#1fb714"),QColor("#006411"),QColor("#562c05"),QColor("#90713a"),
            QColor("#c0c0c0"),QColor("#808080"),QColor("#404040"),QColor("#000000")};
        QColor pickedColor;
        QGridLayout colorGrid;
        std::array<QPushButton, nColors> colorButtons;
        void setColor(QColor newColor);
};

class ZoomControl : public QWidget {
    Q_OBJECT
    public:
        ZoomControl(QWidget* parent = nullptr);
        ~ZoomControl();

    public slots:
        void decreaseZoomClicked();
        void increaseZoomClicked();

    signals:
        void zoomValueChanged(int newZoomValue);

    private:
        int zoomValue{100};
        int stepSize{25}, minVal{25}, maxVal{400};
        QHBoxLayout zoomLayout;
        QPushButton increaseButton;
        QPushButton zoomButton;
        QPushButton decreaseButton;
        void setZoomValue(int newZoomValue);
};

class DrawingToolsMenu : public QWidget {
    Q_OBJECT
    public:
        DrawingToolsMenu(QWidget* parent = nullptr);
        ~DrawingToolsMenu();
        ZoomControl* getZoomControlP();
        SimpleColorDialog* getBackgroundColorDialog();
        SimpleColorDialog* getPenColorDialog();

    public slots:
        void updateBackgroundColorButton(QColor newColor);
        void updatePenColorButton(QColor newColor);

    private:
        QVBoxLayout menuLayout;
        QPushButton changeFullScreen;
        QHBoxLayout drawingControlsLayout;
        QPushButton backgroundColorButton;
        SimpleColorDialog backgroundColorDialog;
        QPushButton penColorButton;
        SimpleColorDialog penColorDialog;
        QPushButton penWidthButton;
        ZoomControl zoomControl;
        void startBackgroundColorDialog();
        void startPenColorDialog();
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
        void updateBackgroundColor(QColor newColor);
        void updatePenColor(QColor newColor);
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
        QAction actionExitApp = QAction(tr("&Exit"), nullptr);

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
        void exitApp();

        SettingsActions settingsActions;
        void configureSettingsMenu();
        void chooseFont();
        void applyFontToDrawingTools();
        void setDefault();

        WritingArea writingArea;
};