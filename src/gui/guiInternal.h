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
#include <QLabel>
#include <QSlider>
#include <QDataStream>

#include <QMap>
#include <QList>

#include <array>
#include <utility>
#include <array>

class PositionControl : public QWidget {
    Q_OBJECT
    public:
        PositionControl(QWidget* parent = nullptr);
        ~PositionControl();

    protected:
        bool event(QEvent *event) override;

    signals:
        void scrollRequested(QPointF delta);

    private:
        QGridLayout gridLayout;
        std::array<QLabel, 9> directionLabels;
        bool scrolling{false};
        int fingerId{-1};
        QPointF previousPoint;
        double traveledDistance{0};
        QPointF applyAcceleration(QPointF newPoint);
};

class PenWidthSliderDialog : public QDialog {
    Q_OBJECT
    public:
        PenWidthSliderDialog(QWidget* parent = nullptr);
        ~PenWidthSliderDialog();

    public slots:
        void penWidthSliderMoved(int newWidth);

    signals:
        void penWidthChanged(int newWidth);

    private:
        int penWidth{4};
        QHBoxLayout sliderLayout;
        QSlider sliderControl;
        QLabel valueLabel;
};

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
        void setZoomValue(int newZoomValue);

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
};

class DrawingToolsMenu : public QWidget {
    Q_OBJECT
    public:
        DrawingToolsMenu(QWidget* parent = nullptr);
        ~DrawingToolsMenu();
        PositionControl* getPositionControl();
        ZoomControl* getZoomControl();
        SimpleColorDialog* getBackgroundColorDialog();
        SimpleColorDialog* getPenColorDialog();
        PenWidthSliderDialog* getPenWidthSliderDialog();

        QPushButton* getChangeFullScreenButton();

    public slots:
        void updateBackgroundColorButton(QColor newColor);
        void updatePenColorButton(QColor newColor);

    private:
        QVBoxLayout menuLayout;
        PositionControl positionControl;
        QPushButton changeFullScreenButton;
        QHBoxLayout drawingControlsLayout;
        QPushButton backgroundColorButton;
        QVBoxLayout backgroundColorButtonLayout;
        QLabel backgroundColorButtonLabel;
        SimpleColorDialog backgroundColorDialog;
        QPushButton penColorButton;
        QVBoxLayout penColorButtonLayout;
        QLabel penColorButtonLabel;
        SimpleColorDialog penColorDialog;
        QPushButton penWidthButton;
        QVBoxLayout penWidthButtonLayout;
        QLabel penWidthButtonLabel;
        PenWidthSliderDialog penWidthDialog;
        ZoomControl zoomControl;
        void startBackgroundColorDialog();
        void startPenColorDialog();
        void startPenWidthDialog();
};

struct LineSegment {
    QPointF start, end;
    QColor color;
    double width;
};

inline QDataStream& operator<<(QDataStream &out, const LineSegment& lineSegment) {
    out << lineSegment.start << lineSegment.end << lineSegment.color << static_cast<qreal>(lineSegment.width);
    return out;
}

inline QDataStream& operator>>(QDataStream &in, LineSegment& lineSegment) {
    qreal width;
    in >> lineSegment.start >> lineSegment.end >> lineSegment.color >> width;
    lineSegment.width = static_cast<double>(width);
    return in;
}

class WritingArea : public QWidget{
    Q_OBJECT
    public:
        WritingArea(QWidget* parent = nullptr);
        ~WritingArea();
        DrawingToolsMenu* getDrawingToolsMenu();
        QMap<qint32, QList<LineSegment>>& getQInternalStore();
        bool getHasUnsavedChanges();
        void setHasUnsavedChanges(bool newVal);
        void setQInternalStore(QMap<qint32, QList<LineSegment>>&& loadedInternalStore);

        void loadFileSession(QString currentWorkingFile);
        void saveFileSession(QString currentWorkingFile);

    protected:
        // void mousePressEvent(QMouseEvent *event) override;
        void tabletEvent(QTabletEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    public slots:
        void updateScroll(QPointF delta);
        void updateBackgroundColor(QColor newColor);
        void updatePenColor(QColor newColor);
        void updatePenWidth(int newWidth);
        void updateZoom(int newZoomValue);


    signals:
        void changeMade();

    private:
        DrawingToolsMenu drawingToolsMenu;
        std::pair<int, int> getCoordinates(QPointF point);
        QPixmap canvasImage;
        double xOrigin{}, yOrigin{}, zoom{1.0};
        int patchSize;
        int nPatches{2000}; // is not consistent with different screens, will be fixed later
        bool hasUnsavedChanges{false};
        QMap<qint32, QList<LineSegment>> qInternalStore;
        int combineIntoIndex(int i, int j);
        QPointF lastPoint;
        QPen canvasPen;
        QColor canvasBackgroundColor{0, 0, 0};

        int processSegment(QPointF startPoint, QPointF finishPoint);
        int recreateCanvas();

        void updateDrawingToolsMenuPosition();
        QSettings appSettings;
        QSettings appSession;
};

class FileActions : public QObject {
    Q_OBJECT
    public:
        QAction actionNewFile = QAction(tr("&New File"), nullptr);
        QAction actionOpenFile = QAction(tr("&Open File"), nullptr);
        QAction actionSaveFile = QAction(tr("&Save File"), nullptr);
        QAction actionSaveFileAs = QAction(tr("S&ave File As"), nullptr);
        QAction actionExitApp = QAction(tr("&Exit"), nullptr);

};

class SettingsActions : public QObject {
    Q_OBJECT
    public:
        QAction actionChooseFont = QAction(tr("&Font"), nullptr);
        QAction actionSetDefault = QAction(tr("&Set Default"), nullptr);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    public slots:
        void configureTitle();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:

        quint32 versionBig{0}, versionSmall{5};

        QSettings appSettings;
        void loadSettings();

        FileActions fileActions;
        void configureFileMenu();
        void newFile();
        void openFile();
        void saveFile();
        void saveFileAs();
        void exitApp();
        bool suggestToSaveOrAbort();

        SettingsActions settingsActions;
        void configureSettingsMenu();
        void chooseFont();
        void applyFontToDrawingTools();
        void setDefault();

        WritingArea writingArea;

        QSettings appSession;
        void loadSession();
        QString currentWorkingFile;

        void changeFullScreen();
};