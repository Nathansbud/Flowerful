#include "SceneMaker.h"

const std::vector<glm::vec4> props = {
    glm::vec4(1, 0, 0, 1),
    glm::vec4(0, 1, 0, 1),
    glm::vec4(0, 0, 1, 1),
    glm::vec4(0.5, 0.5, 0.5, 1),
    glm::vec4(0.5, 0.5, 0, 1),
    //glm::vec4(0.1, 0, 0, 1)
};

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> f(0, 1);
std::uniform_int_distribution<> colorIdx(0, props.size() - 1);
std::uniform_int_distribution<> mushIdx(0, 3);

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

    // make primitives
    ScenePrimitive mushtop_prim = ScenePrimitive{.type=PrimitiveType::PRIMITIVE_MUSHTOP, .material = shroomterial};
    ScenePrimitive mushstem_prim = ScenePrimitive{.type=PrimitiveType::PRIMITIVE_CYLINDER, .material = stemterial};

    // make shrinking matrix
    glm::mat4 scaleall = glm::mat4({0.2, 0, 0, 0}, {0, 0.2, 0, 0}, {0, 0, 0.2, 0}, {0, 0, 0, 1});

    float h = heights[variant];
    float cw = capwidths[variant];
    float ch = capheights[variant];
    // init cap ctms

    glm::mat4 ctm_var4;
    glm::mat4 ictm_var4;
    glm::mat4 nictm_var4;
    RenderShapeData mushtop_var4;

    glm::mat4 ctm = glm::mat4({cw, 0, 0, 0}, {0, ch, 0, 0}, {0, 0, cw, 0}, {xOffset, h, zOffset, 1});
    glm::mat4 ictm = glm::inverse(ctm);
    glm::mat4 nictm = glm::inverse(glm::transpose(glm::mat3(ctm)));

    RenderShapeData mushtop = RenderShapeData{.primitive = mushtop_prim, .ctm = scaleall * ctm, .ictm = ictm, .nictm = nictm};

    // init stem ctms
    glm::mat4 sctm = glm::mat4({1/3.0, 0, 0, 0}, {0, h, 0, 0}, {0, 0, 1/3.0, 0}, {xOffset, h/2, zOffset, 1});
    glm::mat4 sictm = glm::inverse(sctm);
    glm::mat4 snictm = glm::inverse(glm::transpose(glm::mat3(sctm)));

    RenderShapeData mushstem = RenderShapeData{
        .primitive = mushstem_prim,
        .ctm = scaleall * sctm,
        .ictm = sictm,
        .nictm = snictm
    };

    // make the mushroom
    mushroom = new MushroomData;
    mushroom->pieces = std::vector<RenderShapeData>{mushtop, mushstem};

    // extra hat!
    if(variant == 3)  {
        ctm_var4 = glm::mat4({cw/2, 0, 0, 0}, {0, ch, 0, 0}, {0, 0, cw/2, 0}, {xOffset, h + 0.8 * ch/2, zOffset, 1});
        ictm_var4 = glm::inverse(ctm_var4);
        nictm_var4 = glm::inverse(glm::transpose(glm::mat3(ctm_var4)));
        RenderShapeData mushtop_var4 = RenderShapeData{
                .primitive = mushtop_prim,
                .ctm = scaleall * ctm_var4,
                .ictm = ictm_var4,
                .nictm = nictm_var4
        };
        mushroom->pieces.push_back(mushtop_var4);
    }

    return mushroom;
}

std::vector<MushroomData*> SceneMaker::generateScene(int gridLength, int gridDistance) {
    std::vector<MushroomData*> mushGrid(gridLength * gridLength);
    std::uniform_real_distribution<> gridOffset(-gridDistance / 2.f, gridDistance / 2.f);
    int hgl = gridLength / 2;
    for(int i = -hgl; i < hgl; i++) {
        for(int j = -hgl; j < hgl; j++) {
            mushGrid[(i + hgl) * gridLength + (j + hgl)] = SceneMaker::generateMushroom(
                mushIdx(gen),
                gridDistance * i + gridOffset(gen),
                gridDistance * j + gridOffset(gen)
            );
        }
    }

    return mushGrid;
}


