#pragma once

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "Shape.h"

class Cone: public Shape
{
public:
    Cone() {};
private:
    void setVertexData();
    void updateParams(int param1, int param2) {
        m_param1 = param1;
        m_param2 = param2 < 3 ? 3 : param2;
    };

    void makeWall(float currentTheta, float nextTheta);
    void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight);
    void makeCapPiece(glm::vec3 leftCurr, glm::vec3 leftNext, glm::vec3 rightCurr, glm::vec3 rightNext);
};
