#pragma once

#include "utils/camera.h"
#include "utils/scenedata.h"
#include "utils/sceneparser.h"

class RayTraceScene {
private:
    int _width;
    int _height;
    Camera _camera;
    const RenderData& _metadata;
public:
    RayTraceScene(int width, int height, Camera cameraState, const RenderData &metaData);

    const int& width() const;
    const int& height() const;
    const SceneGlobalData& getGlobalData() const;
    const std::vector<RenderShapeData>& getShapes() const;
    const std::vector<SceneLightData>& getLights() const;
    const std::map<std::string, TextureData>& getTextures() const;
    const Camera& getCamera() const;
};
