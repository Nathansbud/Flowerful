#version 330 core

layout(location = 0) in vec3 objPosition;
layout(location = 1) in vec3 objNormal;
layout(location = 2) in vec2 objUV;

out vec3 worldPosition;
out vec3 worldNormal;

out vec2 uvPos;

uniform mat4 modelMatrix;
uniform mat3 normMatrix;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
    worldPosition = vec3(modelMatrix * vec4(objPosition, 1));
    worldNormal = normMatrix * objNormal;

    gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(objPosition, 1);
    uvPos = objUV;
}
