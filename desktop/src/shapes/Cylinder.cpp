#include "Cylinder.h"

void Cylinder::makeTile(
    glm::vec3 topLeft,
    glm::vec3 topRight,
    glm::vec3 bottomLeft,
    glm::vec3 bottomRight
) {
    // Task 5: Implementthe makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    glm::vec3 zeroY(1, 0, 1);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft) * zeroY);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft) * zeroY);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight) * zeroY);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft) * zeroY);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight) * zeroY);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight) * zeroY);
    insertVec2(m_vertexData, glm::vec2(0, 0));
}

void Cylinder::makeCapPiece(
    glm::vec3 leftCurr,
    glm::vec3 leftNext,
    glm::vec3 rightCurr,
    glm::vec3 rightNext,
    bool isTop
) {
    glm::vec3 norm(0, 1 * (isTop ? 1 : -1), 0);
    insertVec3(m_vertexData, leftNext);
    insertVec3(m_vertexData, norm);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, leftCurr);
    insertVec3(m_vertexData, norm);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, rightNext);
    insertVec3(m_vertexData, norm);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, leftCurr);
    insertVec3(m_vertexData, norm);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, rightCurr);
    insertVec3(m_vertexData, norm);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, rightNext);
    insertVec3(m_vertexData, norm);
    insertVec2(m_vertexData, glm::vec2(0, 0));
}

void Cylinder::makeWall(float currentTheta, float nextTheta) {
    glm::vec3 onlyY(0, 1, 0);
    glm::vec3 onlyNY(0, -1, 0);

    for(int i = 0; i < m_param1; i++) {
        float topStep = i / (float) m_param1;
        float nextStep = (i + 1) / (float) m_param1;

        glm::vec3 left = {
            0.5f * glm::sin(currentTheta),
            0.5f,
            0.5f * glm::cos(currentTheta)
        };

        glm::vec3 right = {
            0.5f * glm::sin(nextTheta),
            0.5f,
            0.5f * glm::cos(nextTheta)
        };

        makeTile(left - onlyY * topStep, right - onlyY * topStep, left - onlyY * nextStep, right - onlyY * nextStep);

        glm::vec3 leftPath = glm::vec3(0, 0.5, 0) - left;
        glm::vec3 rightPath = glm::vec3(0, 0.5, 0) - right;

        makeCapPiece(left + leftPath * nextStep, right + rightPath * nextStep, left + leftPath * topStep, right + rightPath * topStep, true);

        leftPath.y *= -1;
        rightPath.y *= -1;
        left.y *= -1;
        right.y *= -1;

        makeCapPiece(left + leftPath * topStep, right + rightPath * topStep, left + leftPath * nextStep, right + rightPath * nextStep, false);
    }
}

void Cylinder::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);
    for(int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWall(currentTheta, nextTheta);
    }
}
