#pragma once

#include "scenedata.h"
#include "sceneparser.h"
#include <vector>
#include <string>
#include <map>
#include <random>
#include "camera.h"
#include "rgba.h"


struct MushroomData {
    int variant;
    SceneMaterial mushcolor2;
    SceneMaterial stemcolor2;
    std::vector<RenderShapeData> pieces;
    std::map<std::string, TextureData> textures;
};

class SceneMaker {

    constexpr static float heights[4] = {1, 0.8, 3, 1.2};
    constexpr static float capwidths[4] = {2.2, 1.5, 1.3, 1.6};
    constexpr static float capheights[4] = {0.8, 2.5, 1.8, 1};

public:
   static MushroomData* generateMushroom(int variant, float xOffset, float zOffset);
   static std::vector<MushroomData*> generateScene(int gridLength, int gridHeight, int gridDistance);
   static void translateMushroom(MushroomData* shroom, float y);
   static void rotateMushroom(MushroomData* shroom, float angle);
   static void bounceMushroom(MushroomData* shroom, float angle);
};

