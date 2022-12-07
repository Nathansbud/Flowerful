#ifndef INTERSECT_H
#define INTERSECT_H

#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <optional>
#include <iostream>
#include "glm/ext/scalar_constants.hpp"
#include "utils/scenedata.h"

// Pack Optional<float, vec3, vec2> representing distance, surface normal, and uv coords
using Intersect = std::tuple<float, glm::vec3, glm::vec2>;

// helpers to get the minimum based on the first in a pair, and return an option accordingly
auto pairCompare = [](auto a, auto b) -> bool {return std::get<0>(a) < std::get<0>(b);};
auto getMin = [](auto& vec) -> std::optional<Intersect> {
    if(vec.size() == 0) return std::nullopt;
    return *std::min_element(vec.begin(), vec.end(), pairCompare);
};

/**
 * @brief Return real roots of a quadratic equation Ax^2 + Bx + C = 0 given A, B, C
 * 
 * @param a: A
 * @param b: B
 * @param c: C
 * @return std::vector<float> holding [0-2] real roots
 */
std::vector<float> solveQuadratic(float a, float b, float c) {
    // check degenerate cases:
    //if a = 0, b != 0, bt + c = 0 has root -c/b
    if(a == 0 && b != 0) return {-c/b};
    // if a = 0, b = 0, only valid if c == 0
    else if(a == 0 && b == 0) {
        if(c == 0) return {0};
        return {};
    }

    // otherwise, solve like normal; check discriminant to see if need to continue (only real roots have intersects)
    float disc = b * b - 4 * a * c;

    // no real roots, empty return
    if(disc < 0) return std::vector<float>();
    // 1 real root, return a vector of size 1
    else if(disc == 0) return {-b / (2*a)};

    // solve for both roots, sort by distance
    float r1 = (-b + sqrt(disc))/(2 * a);
    float r2 = (-b - sqrt(disc))/(2 * a);
    return {r1, r2};
}

/**
 * @brief Return (optional) intersection point and surface normal for a cylinder h = 1, r = 0.5, centered at the origin
 * 
 * @param eye: Ray origin in object space
 * @param dir: Ray direction in object space
 * @return Intersect
 */
std::optional<Intersect> intersectCylinder(glm::vec3& eye, glm::vec3& dir) {
    std::vector<Intersect> candidates;
    
    glm::vec3 dirP = dir * glm::vec3(1, 0, 1);
    glm::vec3 eyeP = eye * glm::vec3(1, 0, 1);
    std::vector<float> quadRoots = solveQuadratic(
        dot(dirP, dirP),
        2 * dot(dirP, eyeP),
        dot(eyeP, eyeP) - 0.5f * 0.5f
    );
    
    for(int r = 0; r < quadRoots.size(); r++) {
        float dist = quadRoots[r];
        // toss negative roots
        if(dist < 0) continue;

        glm::vec3 inter = dist * dir + eye;
        if(inter.y <= 0.5f && inter.y >= -0.5f) {
            float ut = atan2(inter.z, inter.x) / (2 * glm::pi<float>());
            // compute surface normal for our cylinder, <2x, 0, 2z>
            candidates.emplace_back(Intersect{dist, glm::vec3(2, 0, 2) * inter, glm::vec2((ut >= 0) - ut, 0.5f + inter.y)});
        }
    }

    // compute plane root; if dir.y == 0, the solution will be imaginary, hence no intersect
    if(dir.y != 0) {
        for(float y : std::vector<float>{-0.5f, 0.5f}) {
            float planeRoot = ((y - eye.y) / dir.y);
            // toss negative roots
            if(planeRoot < 0) continue;

            glm::vec3 inter = planeRoot * dir + eye;
            if((inter.x * inter.x + inter.z * inter.z) <= 0.5f * 0.5f) {
                // plane normal is just <0, {-1, 1}, 0>
                glm::vec2 uv = y > 0 ? glm::vec2(inter.x + 0.5f, -inter.z + 0.5f) : glm::vec2(inter.x + 0.5f, inter.z + 0.5f);
                candidates.emplace_back(Intersect{planeRoot, glm::vec3(0, 2 * y, 0), uv});
            }
        }
    }

    return getMin(candidates);
}

/**
 * @brief Return (optional) intersection point, surface normal, and UV coordinate for sphere centered at the origin, r = 0.5
 * 
 * @param eye: Ray origin in object space
 * @param dir: Ray direction in object space
 * @return Intersect
 */
std::optional<Intersect> intersectSphere(glm::vec3& eye, glm::vec3& dir) {
    const float RADIUS = 0.5f;
    std::vector<float> roots = solveQuadratic(
        glm::dot(dir, dir),
        2 * glm::dot(eye, dir),
        glm::dot(eye, eye) - (RADIUS * RADIUS)
    );

    float candidateR = -1;
    for(float& r : roots) {
        if((candidateR == -1 && r >= 0) || (r >= 0 && r < candidateR)) {
            candidateR = r;
        }
    }

    if (candidateR == -1) return std::nullopt;

    glm::vec3 inter = dir * candidateR + eye;

    float ut = atan2(inter.z, inter.x) / (2 * glm::pi<float>());
    float v = 0.5f + asin(inter.y / RADIUS) / glm::pi<float>();

    return Intersect{
        candidateR,
        2.0f * inter,
        glm::vec2((v == 0 || v == 1) ? 0.5f : (ut >= 0) - ut, v)
    };
}

/**
 * @brief Return (optional) intersection point, surface normal, and UV coordinate for cone centered at the origin, r = 0.5, h = 1
 * 
 * @param eye: Ray origin in object space
 * @param dir: Ray direction in object space
 * @return Intersect
 */
std::optional<Intersect> intersectCone(glm::vec3& eye, glm::vec3& dir) {
    std::vector<Intersect> candidates;

    glm::vec3 dirYScale = glm::vec3(dir.x, -0.25*dir.y, dir.z);
    glm::vec3 eyeYScale = glm::vec3(eye.x, -0.25*eye.y, eye.z);

    std::vector<float> conicRoots = solveQuadratic(
        glm::dot(dir, dirYScale),
        2 * glm::dot(eye, dirYScale) - dirYScale.y,
        glm::dot(eye, eyeYScale) - eyeYScale.y - 1.0f/16
    );

    for(float r : conicRoots) {
        if(r >= 0) {
            glm::vec3 inter = eye + dir * r;
            if(inter.y <= 0.5 && inter.y >= -0.5) {
                float ut = atan2(inter.z, inter.x) / (2 * glm::pi<float>());

                // cone gradient is <8x, -2y - 1, 8z>, coming from f(x, y, z) = 4x^2 + 4z^2 - y^2 - y - 1/4
                candidates.emplace_back(
                    Intersect{
                        r,
                        glm::vec3(8, -2, 8) * inter + glm::vec3(0, 1, 0),
                        glm::vec2((ut >= 0) - ut, 0.5f + inter.y)
                    }
                );
            }
        }
    }

    if(dir.y != 0) {
        float planeRoot = (-0.5 - eye.y) / dir.y;
        if(planeRoot >= 0) {
            glm::vec3 inter = planeRoot * dir + eye;
            if(inter.x * inter.x + inter.z * inter.z <= 0.25) {
                candidates.emplace_back(Intersect{planeRoot, glm::vec3(0, -1, 0), glm::vec2(inter.x + 0.5f, inter.z + 0.5f)});
            }
        }
    }

    return getMin(candidates);
}

// helper to index into a circular list
auto wrappedMod = [](auto a, auto b) -> int {return (b + (a % b)) % b;};

/**
 * @brief Return (optional) intersection point, surface normal, and UV coordinate for a unit cube centered at the origin
 * 
 * @param eye: Ray origin in object space
 * @param dir: Ray direction in object space
 * @return Intersect
 */
std::optional<Intersect> intersectCube(glm::vec3& eye, glm::vec3& dir) {
    std::vector<Intersect> candidates;

    // iterate over all 6 possible plane intersections
    for(int i = 0; i < 3; i++) {
        if(dir[i] != 0) {
            for(int s : std::vector<int>{1, -1}) {
                // plane equation is {x, y, z} = {0.5, -0.5}, so roots are as follows
                float r = (s * 0.5f - eye[i]) / dir[i];
                if(r >= 0) {
                    glm::vec3 inter = eye + r * dir;
                    // check other two coordinates to make sure within the plane range
                    float dim1 = inter[wrappedMod(i + 1, 3)];
                    float dim2 = inter[wrappedMod(i - 1, 3)];
                    if(abs(dim1) <= 0.5 && abs(dim2) <= 0.5) {
                        // norm is <0, 1, 0> for {x, y, z} = 0.5, <0, -1, 0> for = -0.5
                        glm::vec3 norm = glm::vec3(0, 0, 0);
                        norm[i] = s;

                        glm::vec2 uv = (i == 0) ? glm::vec2(-s * dim2 + 0.5f, dim1 + 0.5f) :
                                       (i == 1) ? glm::vec2(dim2 + 0.5f, -s * dim1 + 0.5f) :
                                                  glm::vec2(s * dim1 + 0.5f, dim2 + 0.5f);

                        candidates.emplace_back(Intersect{r, norm, uv});
                    }
                }
            }
        }
    }
    
    return getMin(candidates);
}

/**
 * @brief Return (optional) intersect of an object primitive
 * 
 * @param t: PrimitiveType of given object to intersect
 * @param eye: Ray origin in object space
 * @param dir: Ray direction in object space
 * @return Intersect 
 */
std::optional<Intersect> intersect(PrimitiveType t, glm::vec3& eye, glm::vec3& dir) {
    switch(t) {
        case PrimitiveType::PRIMITIVE_CUBE:     return intersectCube(eye, dir);
        case PrimitiveType::PRIMITIVE_CYLINDER: return intersectCylinder(eye, dir);
        case PrimitiveType::PRIMITIVE_SPHERE:   return intersectSphere(eye, dir);
        case PrimitiveType::PRIMITIVE_CONE:     return intersectCone(eye, dir);
        // Unimplemented primitives
        case PrimitiveType::PRIMITIVE_MESH:
        case PrimitiveType::PRIMITIVE_TORUS:
            return std::nullopt;
            break;
    }
    return std::nullopt;
}

#endif
