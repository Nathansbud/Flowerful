#include "raytracescene.h"
#include "utils/sceneparser.h"

RayTraceScene::RayTraceScene(int width, int height, Camera cameraState, const RenderData &metaData) :
    _metadata(metaData) {
    this->_width = width;
    this->_height = height; 
    this->_camera = cameraState;
}

const int& RayTraceScene::width() const {return this->_width;}
const int& RayTraceScene::height() const {return this->_height;}

const SceneGlobalData& RayTraceScene::getGlobalData() const {return this->_metadata.globalData;}
const std::vector<RenderShapeData>& RayTraceScene::getShapes() const {return this->_metadata.shapes;}
const std::vector<SceneLightData>& RayTraceScene::getLights() const {return this->_metadata.lights;}
const std::map<std::string, TextureData>& RayTraceScene::getTextures() const {return this->_metadata.textures;}
const Camera& RayTraceScene::getCamera() const {return this->_camera;}
