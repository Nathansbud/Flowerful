#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>

#include "./utils/camera.h"
#include "./utils/sceneparser.h"
#include "./utils/shaderloader.h"
#include "./utils/SceneMaker.h"
#include "settings.h"

// Might be better served in an enum
enum ShapeID {
    CUBE_ID = 0,
    SPHERE_ID = 1,
    CYLINDER_ID = 2,
    CONE_ID = 3,
    MUSH_ID = 4,
};

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

    void songChanged();

    // EC
    void raytrace();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Rotation updates
    float pendingRotX = 0;
    float pendingRotY = 0;

    Camera camera;
    RenderData renderData;

    std::vector<MushroomData*> mushGrid;

    // GL variables
    GLuint m_shader;
    GLuint primitive_vaos[5];                           // All primtives are VBOs of [(vertex, normal), ...]
    GLuint primitive_vbos[5];
    std::vector<float> primitive_data[5];

    // Post-processing data
    GLuint pp_shader;

    float pp_fbo_width;
    float pp_fbo_height;

    GLuint pp_fbo_default;

    GLuint pp_texture;
    GLuint pp_rbo;
    GLuint pp_fbo;

    // fullscreen quad
    GLuint screen_vbo;
    GLuint screen_vao;

    // fullscreen quad with
    std::vector<GLfloat> screen_quad = {
          -1, 1, 0,     0, 1,
          -1, -1, 0,    0, 0,
          1, -1, 0,     1, 0,
          1, 1, 0,      1, 1,
          -1, 1, 0,     0, 1,
          1, -1, 0,     1, 0,
    };

    // Helper fns
    void loadLights();
    void refreshShapes();
    void refreshCamera();
    void refreshFBOs();

    // Settings
    Settings storedSettings = settings;
    bool initialized = false;

    // Audio
    QMediaPlayer* player = nullptr;
    QAudioOutput* audioOutput = nullptr;

    // Device Correction Variables
    int m_devicePixelRatio;
};
