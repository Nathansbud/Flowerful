#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"
#include "QtGui/qimage.h"

#include <chrono>
#include <memory>
#include <iostream>

void loadShapes(std::vector<RenderShapeData>& shapes, SceneNode* node, glm::mat4 ctm) {
    for(SceneTransformation* s : node->transformations) {
        switch(s->type) {
            case TransformationType::TRANSFORMATION_MATRIX:
                ctm *= s->matrix;
                break;
            case TransformationType::TRANSFORMATION_ROTATE:
                ctm *= glm::rotate(s->angle, s->rotate);
                break;
            case TransformationType::TRANSFORMATION_SCALE:
                ctm *= glm::scale(s->scale);
                break;
            case TransformationType::TRANSFORMATION_TRANSLATE:
                ctm *= glm::translate(s->translate);
                break;
        }
    }

    for(ScenePrimitive* p : node->primitives) {
        shapes.emplace_back((RenderShapeData){
            .primitive = *p,
            .ctm = ctm,
            .ictm = glm::inverse(ctm),
            .nictm = glm::transpose(glm::inverse(glm::mat3(ctm)))
        });
    }

    if(!node->children.empty()) {
        for(SceneNode* n : node->children) {
            loadShapes(shapes, n, ctm);
        }
    }
}

/**
 * @brief Loads a texture image specified by a given filepath into a texture struct
 * @param filepath: path to texture image on disk
 * @param t: Texture struct to store file data in
 * @return True if successfully loads image, False otherwise.
 */
bool loadImageFromFile(const std::string &filepath, TextureData& t) {
    QImage textureImg;
    if (!textureImg.load(QString::fromStdString(filepath))) {
        std::cout << "Failed to load in texture from filepath: " << filepath << std::endl;
        return false;
    }

    textureImg = textureImg.convertToFormat(QImage::Format_RGBX8888);

    t.width = textureImg.width();
    t.height = textureImg.height();

    QByteArray bytes = QByteArray::fromRawData((const char*) textureImg.bits(), textureImg.sizeInBytes());

    t.textureMap.clear();
    t.textureMap.reserve(t.width * t.height);
    for(int i = 0; i < bytes.size() / 4.f; i++) {
        t.textureMap.push_back(RGBA{(std::uint8_t) bytes[4*i], (std::uint8_t) bytes[4*i+1], (std::uint8_t) bytes[4*i+2], (std::uint8_t) bytes[4*i+3]});
    }

    return true;
}

/**
 * @brief Load in all textures used by shapes in a scene
 * @param textures: Map to load textures into (filename -> texture)
 * @param shapes: Shapes to load textures from
 */
bool loadTextures(std::map<std::string, TextureData>& textures, std::vector<RenderShapeData>& shapes) {
    for(RenderShapeData& s : shapes) {
        // if we haven't loaded a given texture, load it!
        std::string& fn = s.primitive.material.textureMap.filename;
        if(s.primitive.material.textureMap.isUsed && fn.length() > 0 && !textures.contains(fn)) {
            TextureData t;
            if(loadImageFromFile(fn, t)) {
                textures.emplace(fn, t);
            } else {
                return false;
            }
        }
    }
    return true;
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    renderData.cameraData = fileReader.getCameraData();
    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();

    renderData.shapes.clear();
    loadShapes(renderData.shapes, fileReader.getRootNode(), glm::mat4(1.0));

    if(!loadTextures(renderData.textures, renderData.shapes)) return false;
    return true;
}
