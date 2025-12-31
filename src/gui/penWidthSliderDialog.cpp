#include <guiInternal.h>

PenWidthSliderDialog::PenWidthSliderDialog(QWidget* parent) : QDialog(parent),
        sliderLayout(this), sliderControl(Qt::Horizontal, this), valueLabel(" 4\n𓂃✍︎", this) {

    sliderControl.setMinimum(1);
    sliderControl.setMaximum(24);
    sliderControl.setSingleStep(1);
    sliderControl.setPageStep(1);
    sliderControl.setTickInterval(1);
    sliderControl.setValue(penWidth);
    
    QString sliderStyle = R"(
        /* 0. Main Widget Settings */
        QSlider:horizontal {
            min-width: 700px;
            min-height: 80px;   /* Increased height to make room for Ticks below */
        }

        /* 1. Groove (The Track) */
        QSlider::groove:horizontal {
            background: white;
            border: 1px solid #ccc;
            height: 20px;       /* Bigger track (was 14px) */
            border-radius: 10px;
            margin: 0px 0;
        }

        /* 2. Sub-Page (The Orange Filled Area) */
        QSlider::sub-page:horizontal {
            background: #FF9500; /* Vivid Orange */
            border: 1px solid #E08400;
            height: 20px;
            border-radius: 10px;
        }

        /* 3. Add-Page (The Empty Area) */
        QSlider::add-page:horizontal {
            background: #ECECEC;
            border: 1px solid #ccc;
            height: 20px;
            border-radius: 10px;
        }

        /* 4. The Handle (Knob) */
        QSlider::handle:horizontal {
            background: white;
            border: 2px solid #5c5c5c;
            width: 48px;        /* 48px is huge and easy to grab */
            height: 48px;
            margin-top: -15px;  /* Math: (48px - 20px) / 2 * -1 = -14px (approx -15 for border) */
            margin-bottom: -15px;
            border-radius: 24px; /* Circle */
        }

        /* 5. Interaction States */
        QSlider::handle:horizontal:hover {
            border-color: #FF9500; /* Orange border on hover */
            background: #fff5e6;   /* Very pale orange tint */
        }

        QSlider::handle:horizontal:pressed {
            background: #FF9500;   /* Solid Orange when dragging */
            border-color: #cc7a00;
        }
    )";
    sliderControl.setStyleSheet(sliderStyle);

    sliderLayout.addWidget(&sliderControl);
    sliderLayout.addWidget(&valueLabel);
    connect(&sliderControl, &QSlider::valueChanged,
        this, &PenWidthSliderDialog::penWidthSliderMoved);

    connect(&sliderControl, &QSlider::sliderReleased,
        this, [this]() { this->accept(); });
}

PenWidthSliderDialog::~PenWidthSliderDialog() {
    
}

void PenWidthSliderDialog::penWidthSliderMoved(int newWidth) {
    penWidth = newWidth;
    sliderControl.setValue(penWidth);
    valueLabel.setText(QString("%1\n𓂃✍︎").arg(newWidth, 2, 10, u' '));
    emit penWidthChanged(newWidth);
}