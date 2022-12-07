#include "raytracer.h"
#include "raytracescene.h"
#include "intersect.h"
#include <iostream>
#include <numeric>
#include <QtConcurrent>


RayTracer::RayTracer(Config config) : m_config(config) {}

// Offset for recursive tracing to prevent self-shadowing
const float OFFSET = 0.001f;

/**
 * @brief Convert vector of unclamped color data [0, 1] to an RGBA value
 *
 * @param illumination: Unclamped color vector
 * @return RGBA
 */
RGBA toRGBA(const glm::vec4 &illumination) {
    return RGBA{
        colorClampNormalized(illumination[0]),
        colorClampNormalized(illumination[1]),
        colorClampNormalized(illumination[2])
    };
}

/**
 * @brief Lerp used between color and material, based on factor alpha
 * @param col: Base color
 * @param mat: Texture color
 * @param alpha: Lerp factor
 * @return vec4 representing the lerped color
 */
glm::vec4 lerp(glm::vec4 mat, glm::vec4 col, float alpha) {
    return col * (1 - alpha) + mat * alpha;
}

/**
 * @brief Render a given scene
 * 
 * @param imageData: Pointer to a grid of pixels
 * @param scene: Scene metadata (width, height, ...)
 */
void RayTracer::render(RGBA* imageData, const RayTraceScene &scene) {
    const Camera& cam = scene.getCamera();
    
    const float k = 1;
    const int SW = scene.width();
    const int SH = scene.height();

    // Compute U and V for our pixel scaling
    const float viewY = 2 * k * tan(cam.getHeightAngle() / 2.0f);
    const float viewX = viewY * cam.getAspectRatio();

    glm::vec4 camEye = cam.getPosition();
    
    const std::vector<RenderShapeData>& shapes = scene.getShapes();

    // Pre-compute the offset pairs, {0.25, 0.5, 0.75} X {0.25, 0.5, 0.75}, for super-sampling
    std::vector<std::pair<float, float>> offsetCross = {
        {0.25f, 0.25f}, {0.25f, 0.5f}, {0.25f, 0.75f},
        {0.5f, 0.25f}, {0.5f, 0.5f}, {0.5f, 0.75f},
        {0.75f, 0.25f}, {0.75f, 0.5f}, {0.75f, 0.75f}
    };

    // Our generic ray processing lambda, with parameters for offsetX and offsetY (for super-sampling, else 0.5 by default)
    auto findIntersect = [&](glm::vec4 direction, glm::vec4 rEye, float thresh) {
        // Initialize to invalid values, to check if an intersect actually occurred
        int shapeIdx = -1;
        Intersect shapeIntersect = {-1, glm::vec3(0), glm::vec2(0)};
        
        // Iterate over our shapes to compute the closest intersect
        for(int s = 0; s < shapes.size(); s++) {
            glm::vec3 shapeDir = glm::vec3(shapes[s].ictm * direction);
            glm::vec3 shapeEye = glm::vec3(shapes[s].ictm * rEye);
            std::optional<Intersect> t = intersect(
                shapes[s].primitive.type,
                shapeEye,
                shapeDir
            );

            if(t.has_value()) {
                if(shapeIdx == -1 || std::get<0>(shapeIntersect) > std::get<0>(t.value())) {
                    shapeIdx = s;
                    shapeIntersect = t.value();
                    // early exit based on threshold to see if intersected (shadowing)
                    if(std::get<0>(t.value()) < thresh) {
                        return std::pair<int, Intersect>{shapeIdx, shapeIntersect};
                    }
                }
            }
        }

        return std::pair<int, Intersect>{shapeIdx, shapeIntersect};

    };

    auto processRay = [&](auto idx, auto offsetX, auto offsetY) {
        int row = idx / SW;
        int col = idx % SW;

        // Generate our X and Y along our view plane
        float normX = -0.5f + (col + offsetX) / scene.width();
        float normY = -0.5f + (scene.height() - 1 - row + offsetY) / scene.height();

        glm::vec4 pixel = glm::vec4(viewX * normX, viewY * normY, -k, 1);
        glm::vec4 primary = glm::normalize(cam.getWorldMatrix() * pixel - camEye);

        auto [shapeIdx, shapeIntersect] = findIntersect(primary, camEye, -INFINITY);

        // If no intersect occurred, return black; else, call Phong with our world-space intersection, normal, and direction,
        // as well our global lighting coefficients, object material, and lights
        if(shapeIdx != -1) {
            auto [t, n, uv] = shapeIntersect;
            return toRGBA(
                computeLighting(
                    glm::vec3(camEye + t * primary),
                    shapes[shapeIdx].nictm * n,
                    -primary,
                    uv,
                    4,
                    findIntersect,
                    shapes[shapeIdx].primitive.material,
                    scene
                )
            );
        }

        return RGBA{0, 0, 0};
    };

    // By default, we shoot our ray through pixel center
    auto processRayDefault = [&](auto idx) { return processRay(idx, 0.5f, 0.5f);};

    // If super-sampling, we shoot rays through multiple points in an image then average
    auto processRaySuper = [&](auto idx) {
        std::vector<RGBA> output;

        for(auto& [offX, offY] : offsetCross) {
            output.emplace_back(processRay(idx, offX, offY));
        }

        float accR = 0;
        float accG = 0;
        float accB = 0;
        for(RGBA& p : output) {
            accR += p.r;
            accG += p.g;
            accB += p.b;
        }

        return clampedColor(accR / offsetCross.size(), accG / offsetCross.size(), accB / offsetCross.size());
    };

    if(this->m_config.enableParallelism) {
        // Create a range object to iterate over, representing the set of pixels in our image
        std::vector<int> range(SW * SH);
        for(int i = 0; i < SW * SH; i++) range[i] = i;

        auto output = this->m_config.enableSuperSample ? 
            QtConcurrent::blockingMapped(range, processRaySuper) : 
            QtConcurrent::blockingMapped(range, processRayDefault);
        
        for(int p = 0; p < SW * SH; p++) {
            imageData[p] = output[p];
        }
    } else {
        for(int p = 0; p < SW * SH; p++) {
            imageData[p] = this->m_config.enableSuperSample ? processRaySuper(p) : processRayDefault(p);
        }
    }
}

/**
 * @brief Run the Phong illumination model for a set of lights on a material given various intersection information
 * @param intersect: World space intersection of a ray and given object
 * @param normal: World space normal of intersect
 * @param directionToCamera: Direction vector to the scene camera
 * @param uv: Texture coordinates for intersected object
 * @param maxBounces: Remaining reflective bounces allowed
 * @param traceRay: Lambda to recursively trace rays for shadowing/reflection/etc
 * @param material: Material for intersected object
 * @param scene: Scene data (lights, globals, textures, ...)
 * @return illumination value for an intersected pixel
 */
glm::vec4 RayTracer::computeLighting(
    glm::vec3 intersect,
    glm::vec3 normal,
    glm::vec3 directionToCamera,
    glm::vec2 uv,
    int maxBounces,
    auto traceRay,
    const SceneMaterial& material,
    const RayTraceScene& scene
) {
    const std::vector<SceneLightData>& lights = scene.getLights();
    const SceneGlobalData& globalData = scene.getGlobalData();
    const std::map<std::string, TextureData>& textures = scene.getTextures();
    const std::vector<RenderShapeData>& shapes = scene.getShapes();

    normal = glm::normalize(normal);
    directionToCamera = glm::normalize(directionToCamera);

    glm::vec4 illumination(0, 0, 0, 1);
    // Add ambient term
    illumination += globalData.ka * material.cAmbient;

    glm::vec4 textureColor = glm::vec4(0, 0, 0, 1);
    float matBlend = material.blend;
    if(m_config.enableTextureMap && material.textureMap.isUsed) {
        const TextureData& td = textures.at(material.textureMap.filename);

        int c = (((int) floorf(uv.x * td.width * material.textureMap.repeatU)) - (uv.x == 1)) % td.width;
        int r = (((int) floorf((1 - uv.y) * td.height * material.textureMap.repeatV)) - (uv.y == 0)) % td.height;

        auto toIll = [&](RGBA c) -> glm::vec4 {
            return glm::vec4(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a);
        };

        glm::vec4 tl = toIll(td.textureMap.at(r * td.width + c));
        if(m_config.enableTextureFilter) {
            int cs = (c == td.width - 1) ? (c - 1) : c + 1;
            int rs = (r == td.height - 1) ? (r - 1) : r + 1;

            glm::vec4 tr = toIll(td.textureMap.at(r * td.width + cs));
            glm::vec4 bl = toIll(td.textureMap.at(rs * td.width + c));
            glm::vec4 br = toIll(td.textureMap.at(rs * td.width + cs));

            textureColor += lerp(lerp(tl, tr, 0.5f), lerp(bl, br, 0.5f), 0.5f);
        } else {
            textureColor += tl;
        }
    } else {
        matBlend = 0;
    }


    for(const SceneLightData& light : lights) {
        switch(light.type) {
            case LightType::LIGHT_DIRECTIONAL: {
                glm::vec3 lightDir = glm::normalize(-light.dir);
                float lightFactor = glm::dot(normal, lightDir);
                lightFactor = (lightFactor > 0) ? lightFactor : 0;

                float toCamera = glm::dot(directionToCamera, glm::reflect(lightDir, normal));
                float cameraFactor = (toCamera < 0) ? -toCamera : 0;

                bool isShadowed = false;
                if(m_config.enableShadow) {
                    auto [shadIdx, shadInter] = traceRay(
                        glm::vec4(lightDir, 0),
                        glm::vec4(intersect + OFFSET * lightDir, 1),
                        INFINITY
                    );

                    isShadowed = shadIdx != -1;
                }

                if(!isShadowed) {
                    illumination += light.color * (
                        // Add diffuse term
                        lightFactor * lerp(textureColor, globalData.kd * material.cDiffuse, matBlend) +
                        // Add specular term
                        globalData.ks * powf(cameraFactor, material.shininess) * material.cSpecular
                     );
                }
                break;
            }
            case LightType::LIGHT_POINT: {
                glm::vec3 dtl = glm::vec3(light.pos) - intersect;

                float ld = glm::length(dtl);
                glm::vec3 directionToLight = glm::normalize(dtl);

                float fatt = fmin(1, 1/glm::dot(light.function, glm::vec3(1, ld, ld * ld)));

                float toLight = glm::dot(normal, directionToLight);
                float lightFactor = toLight > 0 ? toLight : 0;

                float toCamera = glm::dot(directionToCamera, glm::reflect(directionToLight, normal));
                float cameraFactor = toCamera < 0 ? -toCamera : 0;

                bool isShadowed = false;
                if(m_config.enableShadow) {
                    auto [shadIdx, shadInter] = traceRay(
                        glm::vec4(directionToLight, 0),
                        glm::vec4(intersect + OFFSET * directionToLight, 1),
                        ld
                    );

                    isShadowed = (shadIdx != -1) && std::get<0>(shadInter) <= ld;
                }

                if(!isShadowed) {
                    illumination += fatt * light.color * (
                        lightFactor * lerp(textureColor, globalData.kd * material.cDiffuse, matBlend) +
                        globalData.ks * powf(cameraFactor, material.shininess) * material.cSpecular
                    );
                }
                break;
            }
            case LightType::LIGHT_SPOT: {
                glm::vec3 dtl = glm::vec3(light.pos) - intersect;

                float ld = glm::length(dtl);
                glm::vec3 directionToLight = glm::normalize(dtl);

                float angleOuter = light.angle;
                float angleInner = light.angle - light.penumbra;

                glm::vec3 spotDir = glm::normalize(glm::vec3(light.dir));

                // normalized, so |L||C| = 1
                float angleX = glm::acos(glm::dot(spotDir, -directionToLight));

                // intensity is full if <= angleInner
                glm::vec4 intensity = light.color;

                // intensity is decreased by intensity factor if angleInner < x <= angleOuter
                if(angleX <= angleOuter && angleX > angleInner) {
                    float ff = (angleX - angleInner) / (angleOuter - angleInner);
                    intensity *= 1 - (-2 * powf(ff, 3) + 3 * powf(ff, 2));
                } else if(angleX > angleOuter) {
                    // intensity is 0 if x > angleOuter
                    intensity = glm::vec4(0);
                }

                float fatt = fmin(1, 1/glm::dot(light.function, glm::vec3(1, ld, ld * ld)));

                float toLight = glm::dot(normal, directionToLight);
                float lightFactor = toLight > 0 ? toLight : 0;

                float toCamera = glm::dot(directionToCamera, glm::reflect(directionToLight, normal));
                float cameraFactor = toCamera < 0 ? -toCamera : 0;

                bool isShadowed = false;
                if(m_config.enableShadow) {
                    auto [shadIdx, shadInter] = traceRay(
                        glm::vec4(directionToLight, 0),
                        glm::vec4(intersect + OFFSET * directionToLight, 1),
                        ld
                    );

                    isShadowed = (shadIdx != -1) && std::get<0>(shadInter) <= ld;
                }

                if(!isShadowed) {
                    illumination += fatt * intensity * (
                        lightFactor * lerp(textureColor, globalData.kd * material.cDiffuse, matBlend) +
                        globalData.ks * powf(cameraFactor, material.shininess) * material.cSpecular
                    );
                }
                break;
            }
            case LightType::LIGHT_AREA:
                break;

        }
    }

    glm::vec4 cRef = material.cReflective;
    if(m_config.enableReflection && maxBounces > 0 && glm::vec3(cRef) != glm::vec3(0)) {
        glm::vec3 rRay = glm::reflect(-directionToCamera, normal);
        glm::vec3 rEye = intersect + OFFSET * rRay;

        auto [refIdx, refIntersect] = traceRay(glm::vec4(rRay, 0), glm::vec4(rEye, 1), -INFINITY);
        if(refIdx != -1) {
            auto [t, n, uv] = refIntersect;
            illumination += globalData.ks * cRef * computeLighting(
                rEye + t * rRay,
                shapes[refIdx].nictm * n,
                -rRay,
                uv,
                maxBounces - 1,
                traceRay,
                shapes[refIdx].primitive.material,
                scene
            );
        }
    }

    return illumination;
}
