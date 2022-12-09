#include "Cone.h"

void Cone::makeTile(
    glm::vec3 leftCurr,
    glm::vec3 leftNext,
    glm::vec3 rightCurr,
    glm::vec3 rightNext
) {
    glm::vec3 noY(1, 0, 1);
    glm::vec3 ySlope = glm::vec3(0, 1, 0);
    glm::vec3 tip = glm::vec3(0, 0.5, 0);

    glm::vec3 lcn = (leftCurr != tip) ? glm::normalize(glm::vec3(8, -2, 8) * leftCurr + glm::vec3(0, 1, 0)) :
                                        glm::normalize(glm::vec3(8, -2, 8) *
                                                       0.5f * ((leftCurr - 0.25f * (leftCurr - rightCurr)) + (leftNext - 0.25f * (leftNext - rightNext))) +
                                                       glm::vec3(0, 1, 0));

    // not used as a tip, can't be special cased
    glm::vec3 lnn = (leftNext != tip) ? glm::normalize(glm::vec3(8, -2, 8) * leftNext + glm::vec3(0, 1, 0)) :
                                        glm::normalize(glm::vec3(8, -2, 8) *
                                                       0.5f * ((leftCurr - 0.25f * (leftCurr - rightCurr)) + (leftNext - 0.25f * (leftNext - rightNext))) +
                                                       glm::vec3(0, 1, 0));

    // not used as a tip, can't be special cased
    glm::vec3 rcn = glm::normalize(glm::vec3(8, -2, 8) * rightCurr + glm::vec3(0, 1, 0));
    glm::vec3 rnn = glm::normalize(glm::vec3(8, -2, 8) * rightNext + glm::vec3(0, 1, 0));

    insertVec3(m_vertexData, leftNext);
    insertVec3(m_vertexData, lnn);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, leftCurr);
    insertVec3(m_vertexData, lcn);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, rightNext);
    insertVec3(m_vertexData, rnn);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, leftCurr);
    insertVec3(m_vertexData, lcn);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, rightCurr);
    insertVec3(m_vertexData, rcn);
    insertVec2(m_vertexData, glm::vec2(0, 0));

    insertVec3(m_vertexData, rightNext);
    insertVec3(m_vertexData, rnn);
    insertVec2(m_vertexData, glm::vec2(0, 0));
}

void Cone::makeCapPiece(
    glm::vec3 leftCurr,
    glm::vec3 leftNext,
    glm::vec3 rightCurr,
    glm::vec3 rightNext
) {
    glm::vec3 norm(0, -1, 0);
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

void Cone::makeWall(float currentTheta, float nextTheta) {
    for(int i = 0; i < m_param1; i++) {
        float topStep = i / (float) m_param1;
        float nextStep = (i + 1) / (float) m_param1;

        glm::vec3 left = {
            0.5f * glm::sin(currentTheta),
            -0.5f,
            0.5f * glm::cos(currentTheta)
        };

        glm::vec3 right = {
            0.5f * glm::sin(nextTheta),
            -0.5f,
            0.5f * glm::cos(nextTheta)
        };

        glm::vec3 leftPath = glm::vec3(0, 0.5, 0) - left;
        glm::vec3 rightPath = glm::vec3(0, 0.5, 0) - right;

        makeTile(left + leftPath * nextStep, right + rightPath * nextStep, left + leftPath * topStep, right + rightPath * topStep);

        leftPath = glm::vec3(0, -0.5, 0) - left;
        rightPath = glm::vec3(0, -0.5, 0) - right;

        makeCapPiece(left + leftPath * topStep, right + rightPath * topStep, left + leftPath * nextStep, right + rightPath * nextStep);
    }
}

void Cone::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);
    for(int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWall(currentTheta, nextTheta);
    }
}
