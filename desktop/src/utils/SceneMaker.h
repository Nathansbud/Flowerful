#pragma once

#include "scenedata.h"
#include "sceneparser.h"
#include <vector>
#include <string>
#include <map>
#include "rgba.h"


struct MushroomData {
    std::vector<RenderShapeData> mushroom1;
    std::vector<RenderShapeData> mushroom2;
    std::vector<RenderShapeData> mushroom3;
    std::vector<RenderShapeData> mushroom4;
    std::map<std::string, TextureData> textures;
};




class SceneMaker {

public:
   static void generateMushrooms(MushroomData &mushrooms);
};

