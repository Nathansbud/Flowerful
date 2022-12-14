#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "realtime.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();

private:
    // Realtime Connectors
    void connectUIElements();
    void connectParam1();
    void connectParam2();
    void connectFar();

    // Flowerful Connectors
    void connectPixel();
    void connectCinematic();
    void connectUploadSong();
    void connectVolume();
    void connectFogColor();

    Realtime *realtime;
    QSlider *p1Slider;
    QSlider *p2Slider;
    QSpinBox *p1Box;
    QSpinBox *p2Box;
    QSlider *farSlider;
    QDoubleSpinBox *farBox;

    // Flowerful UI
    QSlider *redSlider;
    QDoubleSpinBox *redBox;

    QSlider *greenSlider;
    QDoubleSpinBox *greenBox;

    QSlider *blueSlider;
    QDoubleSpinBox *blueBox;

    QSlider *pixelSlider;
    QSpinBox *pixelBox;
    QCheckBox *pixelate;
    QCheckBox *animatePixels;

    QCheckBox *cinematic;

    QPushButton *uploadSong;
    QCheckBox *mute;

    QSlider *volumeSlider;
    QSpinBox *volumeBox;

private slots:
    // Realtime Slots
    void onValChangeP1(int newValue);
    void onValChangeP2(int newValue);
    void onValChangeFarSlider(int newValue);
    void onValChangeFarBox(double newValue);

    // Flowerful Slots
    void onValChangeFogRedSlider(int newValue);
    void onValChangeFogRedBox(double newValue);
    void onValChangeFogGreenSlider(int newValue);
    void onValChangeFogGreenBox(double newValue);
    void onValChangeFogBlueSlider(int newValue);
    void onValChangeFogBlueBox(double newValue);

    void onValChangePixelSlider(int newValue);
    void onValChangePixelBox(int newValue);

    void onPixelate();
    void onAnimatePixels();
    void onCinematic();
    void onMute();

    void onUploadSong();
    void onValChangeVolumeSlider(int newValue);
    void onValChangeVolumeBox(int newValue);

};
