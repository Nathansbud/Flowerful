#include "SceneMaker.h"

void SceneMaker::generateMushrooms(MushroomData &mushrooms) {
    SceneMaterial shroomterial = SceneMaterial{.cAmbient = glm::vec4(0, 1, 0, 1), .cDiffuse = glm::vec4(0, 0, 1, 1),
                                                .cSpecular = glm::vec4(1, 0, 0, 1), .shininess = 0.3};
    SceneMaterial stemterial = SceneMaterial{.cAmbient = glm::vec4(0.5, 0.5, 0.5, 1), .cDiffuse = glm::vec4(0.5, 0.5, 0, 1),
                                                .cSpecular = glm::vec4(0.1, 0, 0, 1), .shininess = 0.3};
    ScenePrimitive mushtop1_prim = ScenePrimitive{.type=PrimitiveType::PRIMITIVE_MUSHTOP, .material = shroomterial};
    ScenePrimitive mushstem1_prim = ScenePrimitive{.type=PrimitiveType::PRIMITIVE_CYLINDER, .material = stemterial};
    glm::mat4 scaleall = glm::mat4({0.2, 0, 0, 0}, {0, 0.2, 0, 0}, {0, 0, 0.2, 0}, {0, 0, 0, 1});

    glm::mat4 ctm = glm::mat4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0.4, 0, 1});
    glm::mat4 ictm = glm::inverse(ctm);
    glm::mat4 nictm = glm::inverse(glm::transpose(ctm));
    RenderShapeData mushtop1 = RenderShapeData{.primitive = mushtop1_prim, .ctm = scaleall * ctm, .ictm = ictm, .nictm = nictm};

    glm::mat4 sctm = glm::mat4({1.0/3.0, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1.0/3.0, 0}, {0, 0, 0, 1});
    glm::mat4 sictm = glm::inverse(sctm);
    glm::mat4 snictm = glm::inverse(glm::transpose(sctm));
    RenderShapeData mushstem1 = RenderShapeData{.primitive = mushstem1_prim, .ctm = scaleall * sctm, .ictm = sictm, .nictm = snictm};


    mushrooms.mushroom1 = std::vector<RenderShapeData>{mushtop1, mushstem1};
}
