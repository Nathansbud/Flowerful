#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 fragColor;

uniform float ka;
uniform float kd;
uniform float ks;

uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform float cShininess;

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

void main() {
    fragColor = ka * cAmbient;
    vec3 normal = normalize(worldNormal);

    vec3 dtc = vec3(cameraPos) - worldPosition;
    float lc = length(dtc);
    vec3 directionToCamera = normalize(dtc);

    for(int i = 0; i < numLights; i++) {
        Light l = lights[i];
        // POINT (it fucked)
        if(l.type == 0) {
            vec3 dtl = l.position - worldPosition;
            float ld = length(dtl);
            vec3 directionToLight = normalize(dtl);

            float fatt = min(1, 1.0/dot(l.function, vec3(1, ld, ld * ld)));
            float lightFactor = dot(normal, directionToLight);
            if(lightFactor < 0) {
                lightFactor = 0;
            }

            fragColor += fatt * l.color * kd * cDiffuse * clamp(lightFactor, 0, 1);
            float cameraFactor = dot(directionToCamera, reflect(directionToLight, normal));
            if(cameraFactor < 0) {
                cameraFactor = -cameraFactor;
            } else {
                cameraFactor = 0;
            }
            cameraFactor = clamp(cameraFactor, 0, 1);

            if(cameraFactor > 0 || (cameraFactor == 0 && cShininess > 0)) {
                fragColor += fatt * l.color * ks * cSpecular * pow(cameraFactor, cShininess);
            }
        }
        // DIRECTIONAL
        else if(l.type == 1) {
            vec3 lightDir = normalize(-l.direction);
            float lightFactor = dot(normal, lightDir);
            if(lightFactor < 0) {
                lightFactor = 0;
            }

            fragColor += l.color * kd * cDiffuse * clamp(lightFactor, 0, 1);

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
        // SPOT
        else {
            vec3 dtl = l.position - worldPosition;
            float ld = length(dtl);
            vec3 directionToLight = normalize(dtl);
            float angleOuter = l.angle;
            float angleInner = l.angle - l.penumbra;

            vec3 spotDir = normalize(vec3(l.direction));
            float angleX = acos(dot(spotDir, -directionToLight));

            vec4 intensity = l.color;
            if(angleX <= angleOuter && angleX > angleInner) {
                float ff = (angleX - angleInner) / (angleOuter - angleInner);
                intensity *= 1 - (-2 * pow(ff, 3) + 3 * pow(ff, 2));
            } else if(angleX > angleOuter) {
                intensity *= 0;
            }

            float fatt = min(1, 1/dot(l.function, vec3(1, ld, ld * ld)));
            float lightFactor = dot(normal, directionToLight);
            if(lightFactor < 0) {
                lightFactor = 0;
            }

            fragColor += fatt * intensity * kd * cDiffuse * clamp(lightFactor, 0, 1);

            float cameraFactor = dot(directionToCamera, reflect(directionToLight, normal));
            if(cameraFactor < 0) {
                cameraFactor = -cameraFactor;
            } else {
                cameraFactor = 0;
            }
            cameraFactor = clamp(cameraFactor, 0, 1);
            if(cameraFactor > 0 || (cameraFactor == 0 && cShininess > 0)) {
                fragColor += fatt * intensity * ks * cSpecular * pow(cameraFactor, cShininess);
            }
        }
    }

    fragColor[3] = 1;
    float fogFactor = clamp((fogMax - lc) / (fogMax - fogMin), 0, 1);
    fragColor = mix(fogColor, fragColor, fogFactor);
}
