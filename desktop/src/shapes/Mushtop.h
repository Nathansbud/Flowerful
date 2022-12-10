#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Shape.h"

class Mush: public Shape
{
public:
    Mush() {};
private:
    void updateParams(int param1, int param2) {
        m_param1 = param1 < 2 ? 2 : param1;
        m_param2 = param2 < 3 ? 3 : param2;
    }
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  float height,
                  bool under);
    void makeWedge(float currTheta, float nextTheta, float height);
    static float getU(glm::vec3 vec) { return atan2(vec.z, vec.x) / (2 * 3.14159265); };
};
