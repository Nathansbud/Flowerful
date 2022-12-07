#version 330 core

in vec2 uvPos;

uniform sampler2D tex;

uniform bool invert;
uniform bool greyscale;

uniform bool sharpen;
uniform bool boxBlur;

uniform int width;
uniform int height;

out vec4 fragColor;

void main() {
    vec4 pix = texture(tex, uvPos);
    float wo = 1/float(width);
    float ho = 1/float(height);

    if(sharpen) {
        pix = vec4(0, 0, 0, 1);
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                pix -= texture(tex, vec2(uvPos.x + wo * i, uvPos.y + ho * j));
            }
        }

        pix += 18 * texture(tex, uvPos);
        pix /= 9.0;
    }

    if(boxBlur) {
        pix = vec4(0, 0, 0, 1);

        float wo = 1/float(width);
        float ho = 1/float(height);

        for(int i = -2; i <= 2; i++) {
            for(int j = -2; j <= 2; j++) {
                pix += texture(tex, vec2(uvPos.x + wo * i, uvPos.y + ho * j));
            }
        }
        pix /= 25.0;
    }

    if(greyscale) {
        float grey = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;
        pix = vec4(grey, grey, grey, 1.0);
    }

    if(invert) {
        pix = 1 - pix;
    }

    fragColor = pix;
}
