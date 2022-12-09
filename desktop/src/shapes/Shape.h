#pragma once

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Shape 
{
public:
    void generateShape(int param1, int param2) {
        m_vertexData.clear();
        updateParams(param1, param2);
        setVertexData();
    };

    std::vector<float> getVertexData() { return m_vertexData; }
protected:
    virtual void setVertexData() {
        std::cout << "using virtual version!!!" << std::endl;
    };
    virtual void updateParams(int param1, int param2) {
        m_param1 = param1;
        m_param2 = param2;
    };

    void insertVec3(std::vector<float> &data, glm::vec3 v) {
        data.push_back(v.x);
        data.push_back(v.y);
        data.push_back(v.z);
    };

    void insertVec2(std::vector<float> &data, glm::vec2 v) {
        data.push_back(v.x);
        data.push_back(v.y);
    };

    static int wrappedMod(int a, int b) {return (b + (a % b)) % b;};

    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;  
};
