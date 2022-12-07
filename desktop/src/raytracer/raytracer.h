#pragma once

#include "../utils/rgba.h"
#include "utils/scenedata.h"
#include <glm/glm.hpp>
#include <optional>

// A forward declaration for the RaytraceScene class

class RayTraceScene;

// A class representing a ray-tracer

class RayTracer
{
public:
    struct Config {
        bool enableShadow        = false;
        bool enableReflection    = false;
        bool enableRefraction    = false;
        bool enableTextureMap    = false;
        bool enableTextureFilter = false;
        bool enableParallelism   = false;
        bool enablePostProcess   = false;
        bool enableSuperSample   = false;
        bool enableAcceleration  = false;
        bool enableDepthOfField  = false;
    };

public:
    RayTracer(Config config);

    // Renders the scene, synchronously or asynchronously (depending on configuration)
    // The ray-tracer will render the scene and fill imageData in-place.
    // @param imageData The pointer to the imageData to be filled.
    // @param scene The scene to be rendered.
    void render(RGBA* imageData, const RayTraceScene &scene);

private:
    glm::vec4 computeLighting(
        glm::vec3 intersect,
        glm::vec3 normal,
        glm::vec3 directionToCamera,
        glm::vec2 uv,
        int maxBounces,
        auto traceRay,
        const SceneMaterial& material,
        const RayTraceScene& scene
    );

    const Config m_config;
};

