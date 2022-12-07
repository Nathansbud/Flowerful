#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <vector>
#include <array>
#include <algorithm>
#include "utils/rgba.h"

/**
 * @brief Clamps a given value between a provided [min, max]
 *
 * @param v: Value to clamp
 * @param min
 * @param max
 * @return int representing v clamped to be between [min, max]
 */
int clamp(int v, int min, int max) {
    return std::min(std::max(v, min), max);
}

/**
 * @brief Convolves passed in separable kernels against an input grid of pixels
 * 
 * @param input: Input pixels to convolve against
 * @param kernels: Separable kernels to convolve against
 * @param norm: Normalizing factor to use
 * @return std::vector<RGBA> output pixel grid after convolving
 */
void blurImage(
    RGBA* input, 
    int m_width,
    int m_height
) {
    int br = 3;
    float norm = 9.0f;
    std::vector<float> triangleWeight(2 * br - 1);
    for(int i = 0; i < br; i++) {
        triangleWeight[i] = triangleWeight[2 * br - 2 - i] = (i + 1)/(float)(br);
    }

    std::array<std::vector<float>, 2> kernels = {triangleWeight, triangleWeight};

    std::vector<URGBA> rowOutput(m_width * m_height);
    std::vector<RGBA> colOutput(m_width * m_height);
    
    std::vector<float>& rowKernel = triangleWeight;
    std::vector<float>& colKernel = triangleWeight;
    
    int radius = rowKernel.size() / 2; 
    
    // apply the row kernel first to create an intermediate stage;
    // clamping is used to handle out-of-bounds behavior, grabbing the final edge pixel;
    // a helper is not used, due to the intermediate use of floats, RGBA, and URGBA grids
    // in the various convolution methods    
    for(int row = 0; row < m_height; row++) {
        for(int col = 0; col < m_width; col++) {
            float pixelR = 0;
            float pixelG = 0;
            float pixelB = 0;

            for(int k = rowKernel.size() - 1; k >= 0; k--) {
                RGBA currPixel = input[
                    clamp(row + (k - radius), 0, m_height - 1) * m_width + 
                    clamp(col, 0, m_width - 1)
                ];
                pixelR += rowKernel[k] * currPixel.r;
                pixelG += rowKernel[k] * currPixel.g;
                pixelB += rowKernel[k] * currPixel.b;
            }
            
            rowOutput[row * m_width + col] = URGBA{pixelR, pixelG, pixelB};
        }
    }

    radius = colKernel.size() / 2;
    for(int row = 0; row < m_height; row++) {
        for(int col = 0; col < m_width; col++) {
            float pixelR = 0;
            float pixelG = 0;
            float pixelB = 0;

            for(int k = colKernel.size() - 1; k >= 0; k--) {
                URGBA currPixel = rowOutput[
                    clamp(row, 0, m_height - 1) * m_width + 
                    clamp(col + (k - radius), 0, m_width - 1)
                ];
                pixelR += colKernel[k] * currPixel.r;
                pixelG += colKernel[k] * currPixel.g;
                pixelB += colKernel[k] * currPixel.b;
            }
            
            input[row * m_width + col] = clampedColor(pixelR / norm, pixelG / norm, pixelB / norm);
        }
    }    
}

#endif
