#include "Sphere.h"

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!
    float phiStep = glm::radians(180.f / (float) m_param1);
    for (int i = 0; i < m_param1; i++) {
        float nextPhi = i * phiStep;
        float currentPhi = (i + 1) * phiStep;

        glm::vec3 topLeft = {
            0.5f * glm::sin(currentPhi) * glm::cos(currentTheta),
            0.5f * glm::cos(currentPhi),
            0.5f * glm::sin(currentTheta) * glm::sin(currentPhi)
        };

        glm::vec3 topRight = {
            0.5f * glm::sin(currentPhi) * glm::cos(nextTheta),
            0.5f * glm::cos(currentPhi),
            0.5f * glm::sin(nextTheta) * glm::sin(currentPhi)
        };

        glm::vec3 bottomLeft = {
            0.5f * glm::sin(nextPhi) * glm::cos(currentTheta),
            0.5f * glm::cos(nextPhi),
            0.5f * glm::sin(currentTheta) * glm::sin(nextPhi)
        };

        glm::vec3 bottomRight = {
            0.5f * glm::sin(nextPhi) * glm::cos(nextTheta),
            0.5f * glm::cos(nextPhi),
            0.5f * glm::sin(nextTheta) * glm::sin(nextPhi)
        };

        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);
    for(int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}
