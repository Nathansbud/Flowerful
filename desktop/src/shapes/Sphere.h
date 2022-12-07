#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "Shape.h"

class Sphere: public Shape
{
public:
    Sphere() {};
private:
    void updateParams(int param1, int param2) {
        m_param1 = param1 < 2 ? 2 : param1;
        m_param2 = param2 < 3 ? 3 : param2;
    }
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeWedge(float currTheta, float nextTheta);
};
