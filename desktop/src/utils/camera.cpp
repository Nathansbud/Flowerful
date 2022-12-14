#include "camera.h"
#include "glm/ext/matrix_transform.hpp"

#include <iostream>

Camera::Camera(int width, int height, float near, float far, SceneCameraData& cameraData)
{
    this->aspectRatio = (float) width / (float) height;
    this->heightAngle = cameraData.heightAngle;
    this->widthAngle = 2 * atan(tan(this->heightAngle / 2.0f) * this->aspectRatio);

    this->pos = cameraData.pos;
    this->look = glm::normalize(cameraData.look);
    this->up = glm::normalize(cameraData.up);

    this->initialPos = pos;
    this->initialLook = look;
    this->initialUp = up;

    computeView();

    this->near = near;
    this->far = far;

    float c = -near/far;

    this->scale = glm::mat4(
        glm::vec4(1 / (far * tan(this->widthAngle / 2)), 0, 0, 0),
        glm::vec4(0, 1 / (far * tan(this->heightAngle / 2)), 0, 0),
        glm::vec4(0, 0, 1 / far, 0),
        glm::vec4(0, 0, 0, 1)
    );

    this->unhinging = glm::mat4(
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1/(1 + c), -1),
        glm::vec4(0, 0, -c/(1 + c), 0)
    );

    this->perspective = Camera::remap * this->unhinging * this->scale;
    this->initialized = true;
}

void Camera::computeView() {
    this->w = glm::normalize(-this->look);
    this->v = glm::normalize(this->up - glm::dot(this->up, w) * w);
    this->u = glm::vec4(glm::normalize(glm::cross(glm::vec3(v), glm::vec3(w))), 0);

    glm::mat4 rot = glm::transpose(glm::mat4(u, v, w, glm::vec4(0, 0, 0, 1)));
    glm::mat4 trans = glm::mat4(1);
    trans[3] = glm::vec4(glm::vec3(-this->pos), 1);

    this->viewMatrix = rot * trans;
    this->worldMatrix = glm::inverse(viewMatrix);
}


void Camera::updateFrustum(float width, float height, float near, float far) {
    this->aspectRatio = width / height;
    this->widthAngle = 2 * atan(tan(this->heightAngle / 2.0f) * this->aspectRatio);

    this->near = near;
    this->far = far;

    float c = -near/far;

    this->scale = glm::mat4(
        glm::vec4(1 / (far * tan(this->widthAngle / 2)), 0, 0, 0),
        glm::vec4(0, 1 / (far * tan(this->heightAngle / 2)), 0, 0),
        glm::vec4(0, 0, 1 / far, 0),
        glm::vec4(0, 0, 0, 1)
    );

    this->unhinging[2][2] = 1/(1 + c);
    this->unhinging[3][2] = -c/(1 + c);
    this->perspective = Camera::remap * this->unhinging * this->scale;
}

void Camera::reset() {
    this->pos = initialPos;
    this->look = initialLook;
    this->up = initialUp;
    computeView();
}

void Camera::rotate(float deltaX, float deltaY) {
    if(!this->initialized) return;
    if(deltaX != 0 || deltaY != 0) {
        glm::mat4 rotX = rodriguez(-deltaX, glm::vec3(0, 1, 0));
        glm::mat4 rotY = rodriguez(-deltaY, this->u);

        this->look = rotY * rotX * this->look;

        computeView();
    }
}

void Camera::cinematic(float forward, float timeStep) {
    if(this->initialized && forward != 0) {
        this->pos += timeStep * forward * glm::vec4(0, 0, 1, 0);
        computeView();
    }
}

void Camera::move(float lf, float uf, float vf, float timeStep) {
    if(!this->initialized) return;
    if(lf != 0 || uf != 0 || vf != 0) {
        this->pos += timeStep * (lf * this->look + uf * this->u + vf * glm::vec4(0, 1, 0, 0));
        computeView();
    }
}
