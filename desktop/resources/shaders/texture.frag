#version 330 core

in vec2 uvPos;
in vec2 imgPos;
uniform sampler2D tex;
//uniform sampler2D bloom;

uniform bool invert;
uniform bool greyscale;

uniform bool sharpen;
uniform bool boxBlur;

uniform int width;
uniform int height;

uniform bool pixelate;
uniform int pixels;
uniform float time;

out vec4 fragColor;

void main() {
    if(pixelate) {
        vec2 uvPrime = floor(uvPos * pixels) / pixels;
        fragColor = texture(tex, uvPrime);
    } else {
        fragColor = texture(tex, uvPos);
    }
}
