#ifndef RGBA_H
#define RGBA_H

#include <cmath>
#include <cstdint>
#include <algorithm>

struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
};

// unbounded RGBA, to allow for accumulations without rounding during intermediate steps
struct URGBA {
    float r;
    float g;
    float b;
    std::uint8_t a = 255;
};

/**
 * @brief Clamps a provided numeric value between [0, 1]
 *
 * @param v: Value to clamp
 * @return uint8_t representing the clamped R/G/B/A value
 */
constexpr uint8_t colorClampNormalized(float f) {
    return (uint8_t) round(fmin(fmax(f, 0.0f), 1.0f) * 255);
}

/**
 * @brief Clamps a provided numeric value between [0, 255]
 *
 * @param v: Value to clamp
 * @return uint8_t representing the clamped R/G/B/A value
 */
constexpr uint8_t colorClamp(auto v) {
    int rounded = (int) round(v);
    return (uint8_t)(std::min(std::max(rounded, 0), 255));
}

/**
 * @brief Clamps a provided R, G, and B value to return a valid color
 *
 * @param r: Red value
 * @param g: Green value
 * @param b: Blue value
 * @return RGBA with clamped R, G, and B values
 */
constexpr RGBA clampedColor(float r, float g, float b) {
    return RGBA{colorClamp(r), colorClamp(g), colorClamp(b)};
}

#endif
