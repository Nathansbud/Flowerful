#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <glm/glm.hpp>

struct Settings {
    // Flowerful Settings
    int songVolume = 50;
    int pixelCount = 128;
    bool pixelate = true;
    bool cinematic = true;
    bool animatePixels = false;
    glm::vec3 fogColor = {0, 0.2, 0.5};

    std::string songFilepath = "../desktop/resources/demo/audio/Nature.mp3";
    float initialBPM = 0;

    // Realtime Settings
    std::string sceneFilepath;
    int shapeParameter1 = 4;
    int shapeParameter2 = 6;

    float nearPlane = 0.1;
    float farPlane = 15;

    bool perPixelFilter = false;
    bool kernelBasedFilter = false;
    bool extraCredit1 = false;
    bool extraCredit2 = false;
    bool extraCredit3 = false;
    bool extraCredit4 = false;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
