const VS_LIGHTING = `#version 300 es
    layout(location=0) in vec3 vertexPosition;
    // layout(location=1) in vec3 vertexNormal;

    out vec4 worldPosition;
    // out vec3 worldNormal;

    // uniform mat4 modelMatrix;
    // uniform mat3 normMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 projMatrix;

    void main() {
        vec4 pos = vec4(vertexPosition, 1);

        // worldPosition = modelMatrix * pos;
        // worldNormal = normMatrix * vertexNormal;

        gl_Position = (projMatrix * viewMatrix) * pos;
            // * modelMatrix) * pos;
    }
`

const FS_LIGHTING = `#version 300 es
    precision highp float;    
    out vec4 fragColor;

    void main() {
        fragColor = vec4(1.0, 0.5, 0.7, 1.0);
    }
`

export {
    VS_LIGHTING, 
    FS_LIGHTING
}
