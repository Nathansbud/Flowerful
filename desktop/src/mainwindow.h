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
    void connectNear();
    void connectFar();
    void connectPerPixelFilter();
    void connectKernelBasedFilter();
    void connectUploadFile();
    void connectExtraCredit();
    void connectRaytrace();

    // Flowerful Connectors
    void connectPixel();
    void connectUploadSong();
    void connectVolume();

    Realtime *realtime;
    QCheckBox *filter1;
    QCheckBox *filter2;
    QPushButton *uploadFile;
    QSlider *p1Slider;
    QSlider *p2Slider;
    QSpinBox *p1Box;
    QSpinBox *p2Box;
    QSlider *nearSlider;
    QSlider *farSlider;
    QDoubleSpinBox *nearBox;
    QDoubleSpinBox *farBox;

    // Extra Credit:
    QCheckBox *ec1;
    QCheckBox *ec2;
    QCheckBox *ec3;
    QCheckBox *ec4;
    QPushButton *raytrace;

    // Flowerful UI
    QSlider *pixelSlider;
    QSpinBox *pixelBox;

    QPushButton *uploadSong;
    QCheckBox *loop;

    QSlider *volumeSlider;
    QSpinBox *volumeBox;

private slots:
    // Realtime Slots
    void onPerPixelFilter();
    void onKernelBasedFilter();
    void onUploadFile();
    void onValChangeP1(int newValue);
    void onValChangeP2(int newValue);
    void onValChangeNearSlider(int newValue);
    void onValChangeFarSlider(int newValue);
    void onValChangeNearBox(double newValue);
    void onValChangeFarBox(double newValue);
    void onExtraCredit1();
    void onExtraCredit2();
    void onExtraCredit3();
    void onExtraCredit4();
    void onRaytrace();

    // Flowerful Slots
    void onValChangePixelSlider(int newValue);
    void onValChangePixelBox(int newValue);

    void onUploadSong();

    void onValChangeVolumeSlider(int newValue);
    void onValChangeVolumeBox(int newValue);
};
