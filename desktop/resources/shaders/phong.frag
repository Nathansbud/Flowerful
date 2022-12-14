#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uvPos;

layout (location = 0) out vec4 fragColor;

uniform float ka;
uniform float kd;
uniform float ks;

uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform float cShininess;
uniform float cBlend;

uniform bool bloomable;
uniform vec4 cameraPos;

struct Light {
    int type;
    vec4 color;
    vec3 direction;
    vec3 position;
    vec3 function;
    float angle;
    float penumbra;
};

uniform Light lights[8];
uniform int numLights;

uniform vec4 fogColor;
uniform float fogMax;
uniform float fogMin;

uniform bool textured;

// 0, 1, 2, 3 = Mushroom Variant, 4 = Ground Texture
uniform sampler2D[5] sceneTextures;
uniform int channel;
uniform bool swapFloor;
uniform float time;

void main() {
    if(textured && channel == 4) {
        if(swapFloor) {
            // might also be nice to just swap?
            vec2 uvUsed = uvPos + 0.1;
            fragColor = texture(sceneTextures[channel], uvUsed);
        } else {
            vec2 uvUsed = uvPos + 0.05;
            fragColor = texture(sceneTextures[channel], uvUsed);
        }
    } else {
        fragColor = ka * cAmbient;
        vec3 normal = normalize(worldNormal);

        vec3 dtc = vec3(cameraPos) - worldPosition;
        float lc = length(dtc);
        vec3 directionToCamera = normalize(dtc);

        for(int i = 0; i < numLights; i++) {
            Light l = lights[i];
            if(l.type == 1) {
                vec3 lightDir = normalize(-l.direction);
                float lightFactor = dot(normal, lightDir);
                if(lightFactor < 0) {
                    lightFactor = 0;
                }

                if(textured) {
                    vec4 texColor = texture(sceneTextures[channel], uvPos);
                    fragColor += l.color * mix(texColor, kd * cDiffuse, cBlend) * clamp(lightFactor, 0, 1);
                } else {
                    fragColor += l.color * kd * cDiffuse * clamp(lightFactor, 0, 1);
                }

                float cameraFactor = dot(directionToCamera, reflect(lightDir, normal));
                if(cameraFactor < 0) {
                    cameraFactor = -cameraFactor;
                } else {
                    cameraFactor = 0;
                }
                cameraFactor = clamp(cameraFactor, 0, 1);

                if(cameraFactor > 0 || (cameraFactor == 0 && cShininess > 0)) {
                    fragColor += l.color * ks * cSpecular * pow(cameraFactor, cShininess);
                }
            }
        }

        fragColor[3] = 1;
        float fogFactor = clamp((fogMax - lc) / (fogMax - fogMin), 0, 1);

        // channel 0 mushroom => rainbow variation
        if(channel == 0 && textured) {
            fragColor = mix(fogColor, mod(fragColor + time, 1.0), fogFactor);
        } else {
            fragColor = mix(fogColor, fragColor, fogFactor);
        }
    }
}
