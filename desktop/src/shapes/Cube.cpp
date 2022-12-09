#include "Cube.h"

glm::vec3 nc(glm::vec3& a, glm::vec3& b, glm::vec3& c) {
    return glm::normalize(glm::cross(b - a, c - a));
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    int stat) {

    float i1 = wrappedMod(stat + 1, 3);
    float i2 = wrappedMod(stat - 1, 3);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, nc(topLeft, bottomLeft, bottomRight));
    insertVec2(m_vertexData, glm::vec2(topLeft[i1] + 0.5, topLeft[i2] + 0.5));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, nc(bottomLeft, bottomRight, topLeft));
    insertVec2(m_vertexData, glm::vec2(bottomLeft[i1] + 0.5, bottomLeft[i2] + 0.5));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, nc(bottomRight, topLeft, bottomLeft));
    insertVec2(m_vertexData, glm::vec2(bottomRight[i1] + 0.5, bottomRight[i2] + 0.5));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, nc(topLeft, bottomRight, topRight));
    insertVec2(m_vertexData, glm::vec2(topLeft[i1] + 0.5, topLeft[i2] + 0.5));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, nc(bottomRight, topRight, topLeft));
    insertVec2(m_vertexData, glm::vec2(bottomRight[i1] + 0.5, bottomRight[i2] + 0.5));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, nc(topRight, topLeft, bottomRight));
    insertVec2(m_vertexData, glm::vec2(topRight[i1] + 0.5, topRight[i2] + 0.5));
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    int stat) {
    for(int i = 0; i < m_param1; i++) {
        for(int j = 0; j < m_param1; j++) {
            makeTile(
                topLeft + (topRight - topLeft) * (i / (float) m_param1) + (bottomLeft - topLeft) * (j / (float) m_param1),
                topLeft + (topRight - topLeft) * ((i + 1) / (float) m_param1) + (bottomLeft - topLeft) * (j / (float) m_param1),
                topLeft + (topRight - topLeft) * (i / (float) m_param1) + (bottomLeft - topLeft) * ((j + 1) / (float) m_param1),
                topLeft + (topRight - topLeft) * ((i + 1) / (float) m_param1) + (bottomLeft - topLeft) * ((j + 1) / (float) m_param1),
                stat
            );
        }
    }
}

void Cube::setVertexData() {
     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f),
              2);

     makeFace(glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f,  0.5f, -0.5f),
              2);

     makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
              glm::vec3( 0.5f, 0.5f, -0.5f),
              glm::vec3(-0.5f, 0.5f,  0.5f),
              glm::vec3( 0.5f, 0.5f,  0.5f),
              1);

     makeFace(glm::vec3(-0.5f, -0.5f,  0.5f),
              glm::vec3( 0.5f, -0.5f,  0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f),
              1);

     makeFace(glm::vec3(0.5f, -0.5f,  0.5f),
              glm::vec3(0.5f,  0.5f,  0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f),
              glm::vec3(0.5f,  0.5f, -0.5f),
              0);

     makeFace(glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f,  0.5f),
              glm::vec3(-0.5f,  0.5f,  0.5f),
              0);
}
