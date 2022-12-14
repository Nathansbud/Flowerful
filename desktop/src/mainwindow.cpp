#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <iostream>

void MainWindow::initialize() {
    realtime = new Realtime;

    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment

    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    QVBoxLayout *mediaControls = new QVBoxLayout();

    vLayout->setAlignment(Qt::AlignTop);
    mediaControls->setAlignment(Qt::AlignTop);

    hLayout->addLayout(vLayout);
    hLayout->addWidget(realtime, 1);
    hLayout->addLayout(mediaControls);

    this->setLayout(hLayout);

    // Create labels in sidebox
    QFont font;
    font.setPointSize(12);
    font.setBold(true); 

    QLabel *far_label = new QLabel(); // Far plane label
    far_label->setText("Fog Maximum:");

    QGroupBox *farLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lfar = new QHBoxLayout();

    farSlider = new QSlider(Qt::Orientation::Horizontal); // Far plane slider
    farSlider->setTickInterval(1);
    farSlider->setMinimum(100);
    farSlider->setMaximum(10000);
    farSlider->setValue(settings.farPlane * 100);

    farBox = new QDoubleSpinBox();
    farBox->setMinimum(1.f);
    farBox->setMaximum(100.f);
    farBox->setSingleStep(0.1f);
    farBox->setValue(settings.farPlane);

    lfar->addWidget(farSlider);
    lfar->addWidget(farBox);
    farLayout->setLayout(lfar);

    QLabel *fog_color_label = new QLabel(); // Far plane label
    fog_color_label->setText("Fog Color (R, G, B):");

    QVBoxLayout *fogColorLayout = new QVBoxLayout(); // horizonal far slider alignment

    QHBoxLayout *lr = new QHBoxLayout();
    QHBoxLayout *lg = new QHBoxLayout();
    QHBoxLayout *lb = new QHBoxLayout();

    redSlider = new QSlider(Qt::Orientation::Horizontal); // Far plane slider
    redSlider->setTickInterval(1);
    redSlider->setMinimum(0);
    redSlider->setMaximum(255);
    redSlider->setValue(settings.fogColor.r * 255);

    redBox = new QDoubleSpinBox();
    redBox->setMinimum(0);
    redBox->setMaximum(255);
    redBox->setSingleStep(1);
    redBox->setValue(settings.fogColor.r * 255);

    greenSlider = new QSlider(Qt::Orientation::Horizontal); // Far plane slider
    greenSlider->setTickInterval(1);
    greenSlider->setMinimum(0);
    greenSlider->setMaximum(255);
    greenSlider->setValue(settings.fogColor.g * 255);

    greenBox = new QDoubleSpinBox();
    greenBox->setMinimum(0);
    greenBox->setMaximum(255);
    greenBox->setSingleStep(1);
    greenBox->setValue(settings.fogColor.g * 255);

    blueSlider = new QSlider(Qt::Orientation::Horizontal); // Far plane slider
    blueSlider->setTickInterval(1);
    blueSlider->setMinimum(0);
    blueSlider->setMaximum(255);
    blueSlider->setValue(settings.fogColor.b * 255);

    blueBox = new QDoubleSpinBox();
    blueBox->setMinimum(0);
    blueBox->setMaximum(255);
    blueBox->setSingleStep(1);
    blueBox->setValue(settings.fogColor.b * 255);

    lr->addWidget(redSlider);
    lr->addWidget(redBox);

    lg->addWidget(greenSlider);
    lg->addWidget(greenBox);

    lb->addWidget(blueSlider);
    lb->addWidget(blueBox);

    fogColorLayout->addLayout(lr);
    fogColorLayout->addLayout(lg);
    fogColorLayout->addLayout(lb);

    vLayout->addWidget(far_label);
    vLayout->addWidget(farLayout);
    vLayout->addWidget(fog_color_label);
    vLayout->addLayout(fogColorLayout);

    cinematic = new QCheckBox();
    cinematic->setText(QStringLiteral("Cinematic Camera"));
    cinematic->setChecked(settings.cinematic);

    QLabel *pixel_label = new QLabel();
    pixel_label->setText("Pixel Count");
    pixel_label->setFont(font);

    QGroupBox *pixelLayout = new QGroupBox();
    QHBoxLayout *lpix = new QHBoxLayout();

    pixelSlider = new QSlider(Qt::Orientation::Horizontal);
    pixelSlider->setTickInterval(1);
    pixelSlider->setMinimum(64);
    pixelSlider->setMaximum(256);
    pixelSlider->setValue(settings.pixelCount);

    pixelBox = new QSpinBox();
    pixelBox->setMinimum(64);
    pixelBox->setMaximum(256);
    pixelBox->setSingleStep(1);
    pixelBox->setValue(settings.pixelCount);

    pixelate = new QCheckBox();
    pixelate->setChecked(settings.pixelate);

    animatePixels = new QCheckBox();
    animatePixels->setText(QStringLiteral("Animate Pixels (Shroom Mode)"));
    animatePixels->setChecked(settings.animatePixels);

    // Adds the slider and number box to the parameter layouts
    lpix->addWidget(pixelate);
    lpix->addWidget(pixelSlider);
    lpix->addWidget(pixelBox);
    pixelLayout->setLayout(lpix);

    uploadSong = new QPushButton();
    uploadSong->setText(QStringLiteral("Upload MP3"));

    QLabel *volume_label = new QLabel();
    volume_label->setText("Volume");
    volume_label->setFont(font);

    QGroupBox *volumeLayout = new QGroupBox();
    QHBoxLayout *lvol = new QHBoxLayout();

    mute = new QCheckBox();
    mute->setChecked(settings.notMuted);

    volumeSlider = new QSlider(Qt::Orientation::Horizontal);
    volumeSlider->setTickInterval(1);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setValue(settings.songVolume);

    volumeBox = new QSpinBox();
    volumeBox->setMinimum(0);
    volumeBox->setMaximum(100);
    volumeBox->setSingleStep(1);
    volumeBox->setValue(settings.songVolume);

    // Adds the slider and number box to the parameter layouts
    lvol->addWidget(mute);
    lvol->addWidget(volumeSlider);
    lvol->addWidget(volumeBox);
    volumeLayout->setLayout(lvol);

    vLayout->addWidget(pixel_label);
    vLayout->addWidget(pixelLayout);
    vLayout->addWidget(animatePixels);

    mediaControls->addWidget(cinematic);
    mediaControls->addWidget(uploadSong);
    mediaControls->addWidget(volume_label);
    mediaControls->addWidget(volumeLayout);

    connectUIElements();
}

void MainWindow::finish() {
    realtime->finish();
    delete(realtime);
}

void MainWindow::connectUIElements() {
    connectFar();
    connectFogColor();
    connectPixel();
    connectCinematic();
    connectUploadSong();
    connectVolume();
//    connectPlay();
//    connectPause();

}

void MainWindow::connectPixel() {
    connect(pixelSlider, &QSlider::valueChanged, this, &MainWindow::onValChangePixelSlider);
    connect(pixelBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangePixelBox);
    connect(pixelate, &QCheckBox::clicked, this, &MainWindow::onPixelate);
    connect(animatePixels, &QCheckBox::clicked, this, &MainWindow::onAnimatePixels);
}

void MainWindow::onPixelate() {
    settings.pixelate = !settings.pixelate;
    realtime->settingsChanged();
}

void MainWindow::onValChangePixelSlider(int newValue) {
    pixelBox->setValue(newValue);
    settings.pixelCount = pixelBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangePixelBox(int newValue) {
    pixelSlider->setValue(newValue);
    settings.pixelCount = pixelBox->value();
    realtime->settingsChanged();
}

void MainWindow::onAnimatePixels() {
    settings.animatePixels = !settings.animatePixels;
    realtime->settingsChanged();
}

void MainWindow::connectCinematic() {
    connect(cinematic, &QCheckBox::clicked, this, &MainWindow::onCinematic);
}

void MainWindow::onCinematic() {
    settings.cinematic = !settings.cinematic;
    realtime->settingsChanged();
}

void MainWindow::connectUploadSong() {
    connect(uploadSong, &QPushButton::clicked, this, &MainWindow::onUploadSong);
}

void MainWindow::onUploadSong() {
    QString songPath = QFileDialog::getOpenFileName(this, tr("Upload MP3"), QDir::homePath(), tr("Scene Files (*.mp3)"));
    if (songPath.isNull()) {
        std::cout << "User selected no song; continuing..." << std::endl;
        return;
    }

    settings.songFilepath = songPath.toStdString();
    std::cout << "Loaded song: \"" << settings.songFilepath << "\"." << std::endl;

    realtime->songChanged();
}

void MainWindow::onMute() {
    settings.notMuted = !settings.notMuted;
    realtime->settingsChanged();
}

void MainWindow::connectVolume() {
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeVolumeSlider);
    connect(volumeBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangeVolumeBox);
    connect(mute,&QCheckBox::clicked, this, &MainWindow::onMute);
}

void MainWindow::onValChangeVolumeSlider(int newValue) {
    volumeBox->setValue(newValue);
    settings.songVolume = volumeBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeVolumeBox(int newValue) {
    volumeSlider->setValue(newValue);
    settings.songVolume = volumeBox->value();
    realtime->settingsChanged();
}

void MainWindow::connectParam1() {
    connect(p1Slider, &QSlider::valueChanged, this, &MainWindow::onValChangeP1);
    connect(p1Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangeP1);
}

void MainWindow::connectParam2() {
    connect(p2Slider, &QSlider::valueChanged, this, &MainWindow::onValChangeP2);
    connect(p2Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangeP2);
}

void MainWindow::connectFogColor() {
    connect(redSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFogRedSlider);
    connect(redBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeFogRedBox);

    connect(greenSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFogGreenSlider);
    connect(greenBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeFogGreenBox);

    connect(blueSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFogBlueSlider);
    connect(blueBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeFogBlueBox);
}

void MainWindow::onValChangeFogRedSlider(int newValue) {
    redSlider->setValue(newValue);
    redBox->setValue(newValue);
    settings.fogColor[0] = redSlider->value() / 255.f;
    realtime->settingsChanged();
}

void MainWindow::onValChangeFogRedBox(double newValue) {
    redBox->setValue(newValue);
    redSlider->setValue(newValue);
    settings.fogColor[0] = redSlider->value() / 255.f;
    realtime->settingsChanged();
}


void MainWindow::onValChangeFogGreenSlider(int newValue) {
    greenSlider->setValue(newValue);
    greenBox->setValue(newValue);
    settings.fogColor[1] = greenSlider->value() / 255.f;
    realtime->settingsChanged();
}

void MainWindow::onValChangeFogGreenBox(double newValue) {
    greenBox->setValue(newValue);
    greenSlider->setValue(newValue);

    settings.fogColor[1] = greenSlider->value() / 255.f;
    realtime->settingsChanged();
}

void MainWindow::onValChangeFogBlueSlider(int newValue) {
    blueSlider->setValue(newValue);
    blueBox->setValue(newValue);
    settings.fogColor[2] = blueSlider->value() / 255.f;
    realtime->settingsChanged();
}

void MainWindow::onValChangeFogBlueBox(double newValue) {
    blueBox->setValue(newValue);
    blueSlider->setValue(newValue);

    settings.fogColor[2] = blueSlider->value() / 255.f;
    realtime->settingsChanged();
}

void MainWindow::connectFar() {
    connect(farSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFarSlider);
    connect(farBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeFarBox);
}

void MainWindow::onValChangeP1(int newValue) {
    p1Slider->setValue(newValue);
    p1Box->setValue(newValue);
    settings.shapeParameter1 = p1Slider->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeP2(int newValue) {
    p2Slider->setValue(newValue);
    p2Box->setValue(newValue);
    settings.shapeParameter2 = p2Slider->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeFarSlider(int newValue) {
    farBox->setValue(newValue/100.f);
    settings.farPlane = farBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeFarBox(double newValue) {
    farSlider->setValue(int(newValue*100.f));
    settings.farPlane = farBox->value();
    realtime->settingsChanged();
}
