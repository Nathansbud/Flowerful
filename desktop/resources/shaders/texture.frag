#version 330 core

in vec2 uvPos;
uniform sampler2D tex;

uniform bool invert;
uniform bool greyscale;

uniform bool sharpen;
uniform bool boxBlur;

uniform int width;
uniform int height;

uniform int pixels;

out vec4 fragColor;

void main() {
    vec2 uvPrime = floor(uvPos * pixels) / pixels;
    fragColor = texture(tex, uvPrime);
}
