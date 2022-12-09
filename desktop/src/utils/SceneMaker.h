#pragma once

#include "scenedata.h"
#include "sceneparser.h"
#include <vector>
#include <string>
#include <map>
#include <random>
#include "rgba.h"


struct MushroomData {
    int variant;
    std::vector<RenderShapeData> pieces;
    std::map<std::string, TextureData> textures;
};

class SceneMaker {

    constexpr static float heights[4] = {1, 0.8, 1.6, 1.2};
    constexpr static float capwidths[4] = {2.2, 1.5, 1.3, 1.6};
    constexpr static float capheights[4] = {0.8, 2.5, 1.8, 1};

public:
   static MushroomData* generateMushroom(int variant, float xOffset, float zOffset);
   static std::vector<MushroomData*> generateScene(int gridLength, int gridDistance);
   //static MushroomData* rotateMushroom()
};

