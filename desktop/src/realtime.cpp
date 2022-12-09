#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCommandLineParser>
#include <QImage>
#include <QSettings>

#include <iostream>
#include <string>
#include <filesystem>

#include "settings.h"
#include "./shapes/Cone.h"
#include "./shapes/Cube.h"
#include "./shapes/Cylinder.h"
#include "./shapes/Shape.h"
#include "./shapes/Sphere.h"
#include "shapes/Mushtop.h"

#include "./raytracer/raytracer.h"
#include "./raytracer/raytracescene.h"

#include "./utils/postprocess.h"

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(settings.songVolume / 100.f);
    player->play();
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    glDeleteProgram(m_shader);
    glDeleteProgram(pp_shader);

    // clear out our VAO/VBOs for each primitive
    glDeleteVertexArrays(5, primitive_vaos);
    glDeleteBuffers(5, primitive_vbos);

    // clear FBO data
    glDeleteTextures(1, &pp_texture);
    glDeleteRenderbuffers(1, &pp_rbo);
    glDeleteFramebuffers(1, &pp_fbo);

    // clear fullscreen quad
    glDeleteVertexArrays(1, &screen_vao);
    glDeleteBuffers(1, &screen_vbo);

    glDeleteTextures(1, &ground_texture);

    for(int i = 0; i < mushGrid.size(); i++) {
        if(mushGrid[i] != nullptr) delete mushGrid[i];
    }

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    m_shader = ShaderLoader::createShaderProgram(
        ":/resources/shaders/phong.vert",
        ":/resources/shaders/phong.frag"
    );

    pp_shader = ShaderLoader::createShaderProgram(
        ":/resources/shaders/texture.vert",
        ":/resources/shaders/texture.frag"
    );

    bpm = 144;

    pp_fbo_default = 2;
    pp_fbo_width = size().width() * m_devicePixelRatio;
    pp_fbo_height = size().height() * m_devicePixelRatio;

    glUseProgram(pp_shader);
    glUniform1i(glGetUniformLocation(pp_shader, "tex"), 0);
    glUniform1i(glGetUniformLocation(pp_shader, "bloom"), 1);
    glUseProgram(0);

    // generate vbo buffers
    glGenBuffers(5, primitive_vbos);
    glGenVertexArrays(5, primitive_vaos);

    refreshShapes();

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &screen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
    glBufferData(GL_ARRAY_BUFFER, screen_quad.size() * sizeof(GLfloat), screen_quad.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &screen_vao);
    glBindVertexArray(screen_vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    refreshFBOs();

    settings.sceneFilepath = "../desktop/resources/scenefiles/flowerful.xml";

    // populates renderData
    sceneChanged();

    // populates mushrooms, loads additional textures
    mushGrid = SceneMaker::generateScene(20, 4);

    std::string path = "../desktop/resources/textures/forest/GroundForest003_COL_VAR1_1k.jpg";
    TextureData& gt = renderData.textures.at(path);
    // TODO: Parse additional textures from mushroom caps!

    glGenTextures(1, &ground_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ground_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gt.width, gt.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, gt.textureMap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Realtime::refreshCamera() {
    if(settings.sceneFilepath != "") {
        camera = Camera(size().width(), size().height(), settings.nearPlane, settings.farPlane, renderData.cameraData);
    }
}

void Realtime::refreshShapes() {
    makeCurrent();

    int p1 = settings.shapeParameter1;
    int p2 = settings.shapeParameter2;

    for(int i = 0; i < 5; i++) {
        switch(i)  {
            case ShapeID::SPHERE_ID: {
                Sphere s = Sphere();
                s.generateShape(p1, p2);
                primitive_data[i] = s.getVertexData();
                break;
            }
            case ShapeID::CUBE_ID: {
                Cube s = Cube();
                s.generateShape(p1, p2);
                primitive_data[i] = s.getVertexData();
                break;
            }
            case ShapeID::CONE_ID: {
                Cone s = Cone();
                s.generateShape(p1, p2);
                primitive_data[i] = s.getVertexData();
                break;
            }
            case ShapeID::CYLINDER_ID: {
                Cylinder s = Cylinder();
                s.generateShape(p1, p2);
                primitive_data[i] = s.getVertexData();
                break;
            }
            case ShapeID::MUSHTOP_ID: {
                Mush s = Mush();
                s.generateShape(p1, p2);
                primitive_data[i] = s.getVertexData();
                break;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, primitive_vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, primitive_data[i].size() * sizeof(GLfloat), primitive_data[i].data(), GL_STATIC_DRAW);

        glBindVertexArray(primitive_vaos[i]);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Realtime::loadLights() {
    int dirNum = 0;

    for(SceneLightData& light : renderData.lights) {
        if(dirNum >= 8) break;

        if(light.type != LightType::LIGHT_AREA) {
            std::string locator = "lights[" + std::to_string(dirNum) + "]";
            glUniform4f(glGetUniformLocation(m_shader, (locator + ".color").c_str()),
                        light.color.x, light.color.y, light.color.z, light.color.a);
            glUniform3f(glGetUniformLocation(m_shader, (locator + ".direction").c_str()),
                        light.dir.x, light.dir.y, light.dir.z);
            glUniform3f(glGetUniformLocation(m_shader, (locator + ".position").c_str()),
                        light.pos.x, light.pos.y, light.pos.z);

            glUniform1i(glGetUniformLocation(m_shader, (locator + ".type").c_str()), (int) light.type);
            glUniform1f(glGetUniformLocation(m_shader, (locator + ".angle").c_str()), light.angle);
            glUniform1f(glGetUniformLocation(m_shader, (locator + ".penumbra").c_str()), light.penumbra);
            glUniform3f(glGetUniformLocation(m_shader, (locator + ".function").c_str()),
                        light.function.x, light.function.y, light.function.z);

            // increase the number of loaded lights
            dirNum++;
        }
    }

    glUniform1i(glGetUniformLocation(m_shader, "numLights"), dirNum);
}


void Realtime::drawShapes(std::vector<RenderShapeData>& shapes) {
    glm::vec4 camPos = camera.getWorldPosition();

    for(RenderShapeData& shape : shapes) {
        int SHAPE_ID = 0;
        switch(shape.primitive.type) {
            case PrimitiveType::PRIMITIVE_CUBE:
                SHAPE_ID = ShapeID::CUBE_ID;
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                SHAPE_ID = ShapeID::CONE_ID;
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                SHAPE_ID = ShapeID::CYLINDER_ID;
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                SHAPE_ID = ShapeID::SPHERE_ID;
                break;
            case PrimitiveType::PRIMITIVE_MUSHTOP:
                SHAPE_ID = ShapeID::MUSHTOP_ID;
            break;
            default:
                continue;
        }

        glBindVertexArray(primitive_vaos[SHAPE_ID]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE, &(shape.ctm[0][0]));
        glUniformMatrix3fv(glGetUniformLocation(m_shader, "normMatrix"), 1, GL_FALSE, &(shape.nictm[0][0]));

        SceneMaterial& mat = shape.primitive.material;
        glUniform4f(glGetUniformLocation(m_shader, "cAmbient"), mat.cAmbient.r, mat.cAmbient.g, mat.cAmbient.b, mat.cAmbient.a);
        glUniform4f(glGetUniformLocation(m_shader, "cDiffuse"), mat.cDiffuse.r, mat.cDiffuse.g, mat.cDiffuse.b, mat.cDiffuse.a);
        glUniform4f(glGetUniformLocation(m_shader, "cSpecular"), mat.cSpecular.r, mat.cSpecular.g, mat.cSpecular.b, mat.cSpecular.a);
        glUniform1f(glGetUniformLocation(m_shader, "cShininess"), mat.shininess);
        glUniform4f(glGetUniformLocation(m_shader, "cameraPos"), camPos.x, camPos.y, camPos.z, 1);
        glUniform1i(glGetUniformLocation(m_shader, "textured"), mat.textureMap.isUsed);

        glDrawArrays(GL_TRIANGLES, 0, primitive_data[SHAPE_ID].size() / 8);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Realtime::paintGL() {
    glBindFramebuffer(GL_FRAMEBUFFER, pp_fbo);
    glViewport(0, 0, pp_fbo_width, pp_fbo_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);

    loadLights();
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, &(camera.getViewMatrix()[0][0]));
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "projMatrix"), 1, GL_FALSE, &(camera.getPerspectiveMatrix()[0][0]));
    glUniform1f(glGetUniformLocation(m_shader, "ka"), renderData.globalData.ka);
    glUniform1f(glGetUniformLocation(m_shader, "kd"), renderData.globalData.kd);
    glUniform1f(glGetUniformLocation(m_shader, "ks"), renderData.globalData.ks);
    glUniform4f(glGetUniformLocation(m_shader, "fogColor"), fogColor.r, fogColor.g, fogColor.b, 1);
    glUniform1f(glGetUniformLocation(m_shader, "fogMax"), 0.8 * settings.farPlane);
    glUniform1f(glGetUniformLocation(m_shader, "fogMin"), settings.nearPlane);

    drawShapes(renderData.shapes);
    for(MushroomData *mush : mushGrid) {
        if(mush != nullptr) {
            if(mush->variant == 3) SceneMaker::rotateMushroom(mush, camera.getLook(), rotate_angle);
            if(mush->variant == 2) SceneMaker::translateMushroom(mush, translate);
            drawShapes(mush->pieces);
        }
    }

    glUseProgram(0);


    // bind default FBO after rendering shapes
    glBindFramebuffer(GL_FRAMEBUFFER, pp_fbo_default);
    glViewport(0, 0, pp_fbo_width, pp_fbo_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render post-processing effect
    glUseProgram(pp_shader);

    glUniform1i(glGetUniformLocation(pp_shader, "width"), pp_fbo_width);
    glUniform1i(glGetUniformLocation(pp_shader, "height"), pp_fbo_height);

    glUniform1i(glGetUniformLocation(pp_shader, "greyscale"), settings.perPixelFilter);
    glUniform1i(glGetUniformLocation(pp_shader, "invert"), settings.extraCredit1);
    glUniform1i(glGetUniformLocation(pp_shader, "boxBlur"), settings.kernelBasedFilter);
    glUniform1i(glGetUniformLocation(pp_shader, "sharpen"), settings.extraCredit2);
    glUniform1i(glGetUniformLocation(pp_shader, "pixels"), settings.pixelCount);
    glUniform1i(glGetUniformLocation(pp_shader, "pixelate"), settings.pixelate);

    glBindVertexArray(screen_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pp_texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}


void Realtime::refreshFBOs() {
    glGenTextures(1, &pp_texture);

    // Realtime post-processing effect land
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pp_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pp_fbo_width, pp_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &pp_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, pp_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, pp_fbo_width, pp_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &pp_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, pp_fbo);

    // attatch both textures to the framebuffer pp_fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pp_texture, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pp_rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, pp_fbo_default);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // clear FBO data
    glDeleteTextures(1, &pp_texture);

    glDeleteRenderbuffers(1, &pp_rbo);
    glDeleteFramebuffers(1, &pp_fbo);

    pp_fbo_width = w * m_devicePixelRatio;
    pp_fbo_height = h * m_devicePixelRatio;

    refreshFBOs();

    camera.updateFrustum(size().width(), size().height(), settings.nearPlane, settings.farPlane);
}

void Realtime::sceneChanged() {
    // only actually reload the scene if a new one was loaded in
    RenderData loadData;
    bool success = SceneParser::parse(settings.sceneFilepath, loadData);
    if(success) {
        initialized = true;
        renderData = std::move(loadData);
        refreshCamera();
        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::songChanged() {
    player->pause();
    player->setSource(QUrl::fromLocalFile(QString::fromStdString(settings.songFilepath)));
    this->window()->setWindowTitle(QStringLiteral("🎵 song name here (need taglib) 🎵"));
    player->play();
}

void Realtime::settingsChanged() {
    if(settings.songVolume != storedSettings.songVolume) {
        audioOutput->setVolume(settings.songVolume / 100.f);
    }

    if(initialized) {
        if(settings.shapeParameter1 != storedSettings.shapeParameter1 ||
            settings.shapeParameter2 != storedSettings.shapeParameter2) {
            refreshShapes();
        }

        if(settings.farPlane != storedSettings.farPlane || settings.nearPlane != storedSettings.nearPlane) {
            camera.updateFrustum(size().width(), size().height(), settings.nearPlane, settings.farPlane);
        }
    }


    update(); // asks for a PaintGL() call to occur
    storedSettings = settings;
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
    if(m_keyMap[Qt::Key_R]) {
        camera.reset();

        // sometimes things get buggy, so...stop all movement!
        m_keyMap[Qt::Key_W]       = false;
        m_keyMap[Qt::Key_A]       = false;
        m_keyMap[Qt::Key_S]       = false;
        m_keyMap[Qt::Key_D]       = false;
        m_keyMap[Qt::Key_Control] = false;
        m_keyMap[Qt::Key_Space]   = false;
    }
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        if(m_mouseDown) {
            pendingRotX = deltaX / 100.f;
            pendingRotY = deltaY / 100.f;
        }

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around

    bool W = m_keyMap[Qt::Key_W];
    bool A = m_keyMap[Qt::Key_A];
    bool S = m_keyMap[Qt::Key_S];
    bool D = m_keyMap[Qt::Key_D];
    bool CTRL = m_keyMap[Qt::Key_Control];
    bool SPACE = m_keyMap[Qt::Key_Space];

    if(!settings.cinematic && (pendingRotX != 0 || pendingRotY != 0)) {
        camera.rotate(pendingRotX, pendingRotY);
        pendingRotX = 0;
        pendingRotY = 0;
    }

    if(!settings.cinematic) {
        camera.move(W * 1 + S * -1, A * -1 + D * 1, SPACE * 1 + CTRL * -1, 5 * deltaTime);
    } else {
        camera.cinematic(W + -S, 5 * deltaTime);
    }

    // bpm = 60; start at 15 at 0 beats, want to get to -15 in 1 beat
    // 1 beat per second --> 30 deg / second --> 30 * delta time
    float maxhop = 0.2;
    float bps = bpm * (1.0/60.0);

    rotate_angle = 90 * bps * deltaTime;

    if(translate_increase) {
        translate = fmin(bps * deltaTime * maxhop, maxhop-translate_total);
        translate_total += translate;
    }
    else {
        translate = fmax(-bps * deltaTime * maxhop, -maxhop-translate_total);
        translate_total += translate;
    }

    if(fabs(translate_total) >= maxhop) translate_increase = !translate_increase;

    update(); // asks for a PaintGL() call to occur
}

void Realtime::raytrace() {
    if(!initialized) return;

    renderData.cameraData.pos = camera.getPosition();
    renderData.cameraData.look = camera.getLook();
    renderData.cameraData.up = camera.getUp();

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("config", "Path of the config file.");
    parser.process(QCoreApplication::instance()->arguments());

    auto positionalArgs = parser.positionalArguments();
    if (positionalArgs.size() != 1) {
        std::cerr << "Could not find a raytracer configuration file. Please provide a path to a config file (.ini) as a command-line argument." << std::endl;
        return;
    }

    QSettings rtSettings(positionalArgs[0], QSettings::IniFormat);

    int width = rtSettings.value("Canvas/width").toInt();
    int height = rtSettings.value("Canvas/height").toInt();

    QImage image = QImage(width, height, QImage::Format_RGBX8888);

    image.fill(Qt::black);
    RGBA *data = reinterpret_cast<RGBA *>(image.bits());

    RayTracer::Config rtConfig{};
    rtConfig.enableShadow        = rtSettings.value("Feature/shadows").toBool();
    rtConfig.enableReflection    = rtSettings.value("Feature/reflect").toBool();
    rtConfig.enableRefraction    = rtSettings.value("Feature/refract").toBool();
    rtConfig.enableTextureMap    = rtSettings.value("Feature/texture").toBool();
    rtConfig.enableTextureFilter = rtSettings.value("Feature/texture-filter").toBool();
    rtConfig.enablePostProcess   = rtSettings.value("Feature/post-process").toBool();
    rtConfig.enableParallelism   = rtSettings.value("Feature/parallel").toBool();
    rtConfig.enableSuperSample   = rtSettings.value("Feature/super-sample").toBool();
    rtConfig.enableAcceleration  = rtSettings.value("Feature/acceleration").toBool();
    rtConfig.enableDepthOfField  = rtSettings.value("Feature/depthoffield").toBool();

    RayTraceScene scene { width, height, camera, renderData };
    RayTracer raytracer { rtConfig };

    raytracer.render(data, scene);

    // Post-process with a blur filter
    if(rtConfig.enablePostProcess && settings.kernelBasedFilter) {
        blurImage(data, width, height);
    }

    QString oImagePath = rtSettings.value("IO/output").toString();
    bool success = image.save(oImagePath);
    if (!success) {
        success = image.save(oImagePath, "PNG");
    }
    if (success) {
        std::cout << "Saved rendered image to \"" << oImagePath.toStdString() << "\"" << std::endl;
    } else {
        std::cerr << "Error: failed to save image to \"" << oImagePath.toStdString() << "\"" << std::endl;
    }
}
