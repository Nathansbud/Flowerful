const VS_LIGHTING = `#version 300 es
    layout(location=0) in vec4 vertexPos;

    void main() {
        gl_Position = vec4(vertexPos);
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
