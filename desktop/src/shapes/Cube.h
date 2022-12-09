#pragma once

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "Shape.h"

class Cube: public Shape
{
public:
    Cube() {};
private:
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  int stat);

    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  int stat);
};
