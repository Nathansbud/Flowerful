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
    QLabel *tesselation_label = new QLabel(); // Parameters label
    tesselation_label->setText("Tesselation");
    tesselation_label->setFont(font);
    QLabel *camera_label = new QLabel(); // Camera label
    camera_label->setText("Camera");
    camera_label->setFont(font);
    QLabel *filters_label = new QLabel(); // Filters label
    filters_label->setText("Filters");
    filters_label->setFont(font);
    QLabel *ec_label = new QLabel(); // Extra Credit label
    ec_label->setText("Extra Credit");
    ec_label->setFont(font);
    QLabel *param1_label = new QLabel(); // Parameter 1 label
    param1_label->setText("Parameter 1:");
    QLabel *param2_label = new QLabel(); // Parameter 2 label
    param2_label->setText("Parameter 2:");
    QLabel *near_label = new QLabel(); // Near plane label
    near_label->setText("Near Plane:");
    QLabel *far_label = new QLabel(); // Far plane label
    far_label->setText("Far Plane:");

    // Create checkbox for per-pixel filter
    filter1 = new QCheckBox();
    filter1->setText(QStringLiteral("Per-Pixel Filter"));
    filter1->setChecked(false);

    // Create checkbox for kernel-based filter
    filter2 = new QCheckBox();
    filter2->setText(QStringLiteral("Kernel-Based Filter"));
    filter2->setChecked(false);

    // Create file uploader for scene file
    uploadFile = new QPushButton();
    uploadFile->setText(QStringLiteral("Upload Scene File"));

    // Creates the boxes containing the parameter sliders and number boxes
    QGroupBox *p1Layout = new QGroupBox(); // horizonal slider 1 alignment
    QHBoxLayout *l1 = new QHBoxLayout();
    QGroupBox *p2Layout = new QGroupBox(); // horizonal slider 2 alignment
    QHBoxLayout *l2 = new QHBoxLayout();

    // Create slider controls to control parameters
    p1Slider = new QSlider(Qt::Orientation::Horizontal); // Parameter 1 slider
    p1Slider->setTickInterval(1);
    p1Slider->setMinimum(1);
    p1Slider->setMaximum(25);
    p1Slider->setValue(1);

    p1Box = new QSpinBox();
    p1Box->setMinimum(1);
    p1Box->setMaximum(25);
    p1Box->setSingleStep(1);
    p1Box->setValue(1);

    p2Slider = new QSlider(Qt::Orientation::Horizontal); // Parameter 2 slider
    p2Slider->setTickInterval(1);
    p2Slider->setMinimum(1);
    p2Slider->setMaximum(25);
    p2Slider->setValue(1);

    p2Box = new QSpinBox();
    p2Box->setMinimum(1);
    p2Box->setMaximum(25);
    p2Box->setSingleStep(1);
    p2Box->setValue(1);

    // Adds the slider and number box to the parameter layouts
    l1->addWidget(p1Slider);
    l1->addWidget(p1Box);
    p1Layout->setLayout(l1);

    l2->addWidget(p2Slider);
    l2->addWidget(p2Box);
    p2Layout->setLayout(l2);

    // Create file uploader for scene file
    raytrace = new QPushButton();
    raytrace->setText(QStringLiteral("Raytrace Current View"));

    // Creates the boxes containing the camera sliders and number boxes
    QGroupBox *nearLayout = new QGroupBox(); // horizonal near slider alignment
    QHBoxLayout *lnear = new QHBoxLayout();
    QGroupBox *farLayout = new QGroupBox(); // horizonal far slider alignment
    QHBoxLayout *lfar = new QHBoxLayout();

    // Create slider controls to control near/far planes
    nearSlider = new QSlider(Qt::Orientation::Horizontal); // Near plane slider
    nearSlider->setTickInterval(1);
    nearSlider->setMinimum(1);
    nearSlider->setMaximum(1000);
    nearSlider->setValue(10);

    nearBox = new QDoubleSpinBox();
    nearBox->setMinimum(0.01f);
    nearBox->setMaximum(10.f);
    nearBox->setSingleStep(0.1f);
    nearBox->setValue(0.1f);

    farSlider = new QSlider(Qt::Orientation::Horizontal); // Far plane slider
    farSlider->setTickInterval(1);
    farSlider->setMinimum(1000);
    farSlider->setMaximum(10000);
    farSlider->setValue(10000);

    farBox = new QDoubleSpinBox();
    farBox->setMinimum(10.f);
    farBox->setMaximum(100.f);
    farBox->setSingleStep(0.1f);
    farBox->setValue(100.f);

    // Adds the slider and number box to the parameter layouts
    lnear->addWidget(nearSlider);
    lnear->addWidget(nearBox);
    nearLayout->setLayout(lnear);

    lfar->addWidget(farSlider);
    lfar->addWidget(farBox);
    farLayout->setLayout(lfar);

    // Extra Credit:
    ec1 = new QCheckBox();
    ec1->setText(QStringLiteral("Invert"));
    ec1->setChecked(false);

    ec2 = new QCheckBox();
    ec2->setText(QStringLiteral("Sharpen"));
    ec2->setChecked(false);

    ec3 = new QCheckBox();
    ec3->setText(QStringLiteral("Extra Credit 3"));
    ec3->setChecked(false);

    ec4 = new QCheckBox();
    ec4->setText(QStringLiteral("Extra Credit 4"));
    ec4->setChecked(false);

    vLayout->addWidget(uploadFile);
    vLayout->addWidget(tesselation_label);
    vLayout->addWidget(param1_label);
    vLayout->addWidget(p1Layout);
    vLayout->addWidget(param2_label);
    vLayout->addWidget(p2Layout);
    vLayout->addWidget(camera_label);
    vLayout->addWidget(near_label);
    vLayout->addWidget(nearLayout);
    vLayout->addWidget(far_label);
    vLayout->addWidget(farLayout);
    vLayout->addWidget(filters_label);
    vLayout->addWidget(filter1);
    vLayout->addWidget(filter2);
    // Extra Credit:
    vLayout->addWidget(ec_label);
    vLayout->addWidget(ec1);
    vLayout->addWidget(ec2);
    vLayout->addWidget(ec3);
    vLayout->addWidget(ec4);
    vLayout->addWidget(raytrace);

    // Media Controls
    QLabel *pixel_label = new QLabel();
    pixel_label->setText("Pixel Count");
    pixel_label->setFont(font);

    QGroupBox *pixelLayout = new QGroupBox();
    QHBoxLayout *lpix = new QHBoxLayout();

    pixelSlider = new QSlider(Qt::Orientation::Horizontal);
    pixelSlider->setTickInterval(1);
    pixelSlider->setMinimum(2);
    pixelSlider->setMaximum(256);
    pixelSlider->setValue(64);

    pixelBox = new QSpinBox();
    pixelBox->setMinimum(2);
    pixelBox->setMaximum(256);
    pixelBox->setSingleStep(1);
    pixelBox->setValue(64);

    pixelate = new QCheckBox();
    pixelate->setChecked(settings.pixelate);

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

    loop = new QCheckBox();
    loop->setText(QStringLiteral("Loop"));
    loop->setChecked(true);

    QGroupBox *volumeLayout = new QGroupBox();
    QHBoxLayout *lvol = new QHBoxLayout();

    volumeSlider = new QSlider(Qt::Orientation::Horizontal);
    volumeSlider->setTickInterval(1);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setValue(50);

    volumeBox = new QSpinBox();
    volumeBox->setMinimum(0);
    volumeBox->setMaximum(100);
    volumeBox->setSingleStep(1);
    volumeBox->setValue(50);

    // Adds the slider and number box to the parameter layouts
    lvol->addWidget(volumeSlider);
    lvol->addWidget(volumeBox);
    volumeLayout->setLayout(lvol);

    mediaControls->addWidget(pixel_label);
    mediaControls->addWidget(pixelLayout);
    mediaControls->addWidget(uploadSong);
    mediaControls->addWidget(loop);
    mediaControls->addWidget(volume_label);
    mediaControls->addWidget(volumeLayout);

    connectUIElements();

    // Set default values of 5 for tesselation parameters
    onValChangeP1(5);
    onValChangeP2(5);

    // Set default values for near and far planes
    onValChangeNearBox(0.1f);
    onValChangeFarBox(10.f);
}

void MainWindow::finish() {
    realtime->finish();
    delete(realtime);
}

void MainWindow::connectUIElements() {
    // Realtime UI
    connectPerPixelFilter();
    connectKernelBasedFilter();
    connectUploadFile();
    connectParam1();
    connectParam2();
    connectNear();
    connectFar();
    connectExtraCredit();
    connectRaytrace();

    // Flowerful UI
    connectPixel();
    connectUploadSong();
    connectVolume();
//    connectMute();
//    connectLoop();
//    connectPlay();
//    connectPause();

}

void MainWindow::connectPixel() {
    connect(pixelSlider, &QSlider::valueChanged, this, &MainWindow::onValChangePixelSlider);
    connect(pixelBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangePixelBox);
    connect(pixelate, &QCheckBox::clicked, this, &MainWindow::onPixelate);
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

void MainWindow::connectVolume() {
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeVolumeSlider);
    connect(volumeBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangeVolumeBox);
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

void MainWindow::connectPerPixelFilter() {
    connect(filter1, &QCheckBox::clicked, this, &MainWindow::onPerPixelFilter);
}

void MainWindow::connectKernelBasedFilter() {
    connect(filter2, &QCheckBox::clicked, this, &MainWindow::onKernelBasedFilter);
}

void MainWindow::connectUploadFile() {
    connect(uploadFile, &QPushButton::clicked, this, &MainWindow::onUploadFile);
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

void MainWindow::connectNear() {
    connect(nearSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeNearSlider);
    connect(nearBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeNearBox);
}

void MainWindow::connectFar() {
    connect(farSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFarSlider);
    connect(farBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeFarBox);
}

void MainWindow::connectExtraCredit() {
    connect(ec1, &QCheckBox::clicked, this, &MainWindow::onExtraCredit1);
    connect(ec2, &QCheckBox::clicked, this, &MainWindow::onExtraCredit2);
    connect(ec3, &QCheckBox::clicked, this, &MainWindow::onExtraCredit3);
    connect(ec4, &QCheckBox::clicked, this, &MainWindow::onExtraCredit4);
}

void MainWindow::connectRaytrace() {
    connect(raytrace, &QPushButton::clicked, this, &MainWindow::onRaytrace);
}

void MainWindow::onPerPixelFilter() {
    settings.perPixelFilter = !settings.perPixelFilter;
    realtime->settingsChanged();
}

void MainWindow::onKernelBasedFilter() {
    settings.kernelBasedFilter = !settings.kernelBasedFilter;
    if(settings.kernelBasedFilter) {
        settings.extraCredit2 = false;
        ec2->setChecked(false);
    }

    realtime->settingsChanged();
}

void MainWindow::onUploadFile() {
    // Get abs path of scene file
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"), QDir::homePath(), tr("Scene Files (*.xml)"));
    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilepath = configFilePath.toStdString();
    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;
    realtime->sceneChanged();
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

void MainWindow::onValChangeNearSlider(int newValue) {
//    nearSlider->setValue(newValue);
    nearBox->setValue(newValue/100.f);
    settings.nearPlane = nearBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeFarSlider(int newValue) {
//    farSlider->setValue(newValue);
    farBox->setValue(newValue/100.f);
    settings.farPlane = farBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeNearBox(double newValue) {
    nearSlider->setValue(int(newValue*100.f));
    settings.nearPlane = nearBox->value();
    realtime->settingsChanged();
}

void MainWindow::onValChangeFarBox(double newValue) {
    farSlider->setValue(int(newValue*100.f));
    settings.farPlane = farBox->value();
    realtime->settingsChanged();
}

// Extra Credit:

void MainWindow::onExtraCredit1() {
    settings.extraCredit1 = !settings.extraCredit1;
    realtime->settingsChanged();
}

void MainWindow::onExtraCredit2() {
    settings.extraCredit2 = !settings.extraCredit2;
    if(settings.extraCredit2) {
        settings.kernelBasedFilter = false;
        filter2->setChecked(false);
    }

    realtime->settingsChanged();
}

void MainWindow::onExtraCredit3() {
    settings.extraCredit3 = !settings.extraCredit3;
    realtime->settingsChanged();
}

void MainWindow::onExtraCredit4() {
    settings.extraCredit4 = !settings.extraCredit4;
    realtime->settingsChanged();
}

void MainWindow::onRaytrace() {
    realtime->raytrace();
}
