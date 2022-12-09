#include "SceneMaker.h"

const std::vector<glm::vec4> props = {
    glm::vec4(1, 0, 0, 1),
    glm::vec4(0, 1, 0, 1),
    glm::vec4(0, 0, 1, 1),
    glm::vec4(0.5, 0.5, 0.5, 1),
    glm::vec4(0.5, 0.5, 0, 1),
    glm::vec4(0.1, 0, 0, 1)
};

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> f(0, 1);
std::uniform_int_distribution<> colorIdx(0, props.size() - 1);

MushroomData* SceneMaker::generateMushroom(int variant, float xOffset, float zOffset) {
    SceneMaterial shroomterial = SceneMaterial{
        .cAmbient = props[colorIdx(gen)],
        .cDiffuse = props[colorIdx(gen)],
        .cSpecular = props[colorIdx(gen)],
        .shininess = (float) f(gen)
    };

    SceneMaterial stemterial = SceneMaterial{
        .cAmbient = props[colorIdx(gen)],
        .cDiffuse = props[colorIdx(gen)],
        .cSpecular = props[colorIdx(gen)],
        .shininess = (float) f(gen)
    };

    MushroomData* mushroom = nullptr;

    ScenePrimitive mushtop1_prim = ScenePrimitive{.type=PrimitiveType::PRIMITIVE_MUSHTOP, .material = shroomterial};
    ScenePrimitive mushstem1_prim = ScenePrimitive{.type=PrimitiveType::PRIMITIVE_CYLINDER, .material = stemterial};

    glm::mat4 scaleall = glm::mat4({0.2, 0, 0, 0}, {0, 0.2, 0, 0}, {0, 0, 0.2, 0}, {0, 0, 0, 1});

    glm::mat4 ctm = glm::mat4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {xOffset, 0.4, zOffset, 1});
    glm::mat4 ictm = glm::inverse(ctm);
    glm::mat4 nictm = glm::inverse(glm::transpose(glm::mat3(ctm)));

    RenderShapeData mushtop1 = RenderShapeData{.primitive = mushtop1_prim, .ctm = scaleall * ctm, .ictm = ictm, .nictm = nictm};

    glm::mat4 sctm = glm::mat4({1.0/3.0, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1.0/3.0, 0}, {xOffset, 0, zOffset, 1});
    glm::mat4 sictm = glm::inverse(sctm);
    glm::mat4 snictm = glm::inverse(glm::transpose(glm::mat3(sctm)));

    RenderShapeData mushstem1 = RenderShapeData{
        .primitive = mushstem1_prim,
        .ctm = scaleall * sctm,
        .ictm = sictm,
        .nictm = snictm
    };

    mushroom = new MushroomData;
    mushroom->pieces = std::vector<RenderShapeData>{mushtop1, mushstem1};
    return mushroom;
}

std::vector<MushroomData*> SceneMaker::generateScene(int gridLength, int gridDistance) {
    std::vector<MushroomData*> mushGrid(gridLength * gridLength);
    std::uniform_real_distribution<> gridOffset(-gridDistance / 2.f, gridDistance / 2.f);
    int hgl = gridLength / 2;
    for(int i = -hgl; i < hgl; i++) {
        for(int j = -hgl; j < hgl; j++) {
            mushGrid[(i + hgl) * gridLength + (j + hgl)] = SceneMaker::generateMushroom(
                1,
                gridDistance * i + gridOffset(gen),
                gridDistance * j + gridOffset(gen)
            );
        }
    }

    return mushGrid;
}


