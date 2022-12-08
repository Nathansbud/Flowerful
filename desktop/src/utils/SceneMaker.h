#pragma once

#include "scenedata.h"
#include "sceneparser.h"
#include <vector>
#include <string>
#include <map>
#include <random>
#include "rgba.h"


struct MushroomData {
    std::vector<RenderShapeData> pieces;
    std::map<std::string, TextureData> textures;
};

class SceneMaker {

public:
   static MushroomData* generateMushroom(int variant, float xOffset, float zOffset);
   static std::vector<MushroomData*> generateScene(int gridLength, int gridDistance);
};

