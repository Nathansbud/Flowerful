#include "Cube.h"

glm::vec3 nc(glm::vec3& a, glm::vec3& b, glm::vec3& c) {
    return glm::normalize(glm::cross(b - a, c - a));
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, nc(topLeft, bottomLeft, bottomRight));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, nc(bottomLeft, bottomRight, topLeft));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, nc(bottomRight, topLeft, bottomLeft));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, nc(topLeft, bottomRight, topRight));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, nc(bottomRight, topRight, topLeft));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, nc(topRight, topLeft, bottomRight));
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    for(int i = 0; i < m_param1; i++) {
        for(int j = 0; j < m_param1; j++) {
            makeTile(
                topLeft + (topRight - topLeft) * (i / (float) m_param1) + (bottomLeft - topLeft) * (j / (float) m_param1),
                topLeft + (topRight - topLeft) * ((i + 1) / (float) m_param1) + (bottomLeft - topLeft) * (j / (float) m_param1),
                topLeft + (topRight - topLeft) * (i / (float) m_param1) + (bottomLeft - topLeft) * ((j + 1) / (float) m_param1),
                topLeft + (topRight - topLeft) * ((i + 1) / (float) m_param1) + (bottomLeft - topLeft) * ((j + 1) / (float) m_param1)
            );
        }
    }
}

void Cube::setVertexData() {
     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f));

     makeFace(glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f,  0.5f, -0.5f));

     makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
              glm::vec3( 0.5f, 0.5f, -0.5f),
              glm::vec3(-0.5f, 0.5f,  0.5f),
              glm::vec3( 0.5f, 0.5f,  0.5f));

     makeFace(glm::vec3(-0.5f, -0.5f,  0.5f),
              glm::vec3( 0.5f, -0.5f,  0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f));

     makeFace(glm::vec3(0.5f, -0.5f,  0.5f),
              glm::vec3(0.5f,  0.5f,  0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f),
              glm::vec3(0.5f,  0.5f, -0.5f));

     makeFace(glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f,  0.5f),
              glm::vec3(-0.5f,  0.5f,  0.5f));
}
