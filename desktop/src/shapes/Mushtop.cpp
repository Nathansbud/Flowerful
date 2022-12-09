#include "Mushtop.h"

void Mush::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight, bool under) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    float sign = under ? -1 : 1;
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, sign * glm::normalize(topLeft));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, sign * glm::normalize(bottomLeft));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, sign * glm::normalize(bottomRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, sign * glm::normalize(topLeft));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, sign * glm::normalize(bottomRight));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, sign * glm::normalize(topRight));
}

void Mush::makeWedge(float currentTheta, float nextTheta, float height) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!
    float phiStep = glm::radians(90.f / (float) m_param1);
    for (int i = 0; i < m_param1; i++) {
        float nextPhi = i * phiStep;
        float currentPhi = (i + 1) * phiStep;

        glm::vec3 topLeft = {
            0.5f * glm::sin(currentPhi) * glm::cos(currentTheta),
            height * glm::cos(currentPhi),
            0.5f * glm::sin(currentTheta) * glm::sin(currentPhi)
        };

        glm::vec3 topRight = {
            0.5f * glm::sin(currentPhi) * glm::cos(nextTheta),
            height * glm::cos(currentPhi),
            0.5f * glm::sin(nextTheta) * glm::sin(currentPhi)
        };

        glm::vec3 bottomLeft = {
            0.5f * glm::sin(nextPhi) * glm::cos(currentTheta),
            height * glm::cos(nextPhi),
            0.5f * glm::sin(currentTheta) * glm::sin(nextPhi)
        };

        glm::vec3 bottomRight = {
            0.5f * glm::sin(nextPhi) * glm::cos(nextTheta),
            height * glm::cos(nextPhi),
            0.5f * glm::sin(nextTheta) * glm::sin(nextPhi)
        };

        bool under = height == 0.5 ? 0 : 1;
        if(!under) makeTile(topLeft, topRight, bottomLeft, bottomRight, under);
        else makeTile(topRight, topLeft, bottomRight, bottomLeft, under);
    }
}

void Mush::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);
    for(int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta, 0.5);
    }
    for(int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta, 0.1);
    }
}
