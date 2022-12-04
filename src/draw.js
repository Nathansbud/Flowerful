import { VS_LIGHTING, FS_LIGHTING } from './shaders/lighting.js'
import { initShaderProgram } from './utils/shaderloader.js'

/*

RESOURCE ZONE:
- Resizing: https://webgl2fundamentals.org/webgl/lessons/webgl-resizing-the-canvas.html

*/

let gl;
let canvas;

let squareVAO;
let squareVBO;

const buffers = {}
const shaders = {}

function initGL() {
    canvas = document.querySelector("#gl-canvas");
    gl = canvas.getContext("webgl2");
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    
    
    const l_shader = initShaderProgram(gl, VS_LIGHTING, FS_LIGHTING);
    shaders['lighting'] = {
        program: l_shader,
        attributes: {
            vertexPos: gl.getAttribLocation(l_shader, "vertexPos")
        },
        uniforms: {
            proj: gl.getUniformLocation(l_shader, "proj"),
            mat: gl.getUniformLocation(l_shader, "mat"),
        }
    }

    const positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    const positions = [
        1.0, 1.0, 
        -1.0, 1.0, 
        1.0, -1.0,
        1.0, 1.0, 
        1.0, -1.0, 
        -1.0, -1.0
    ]
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    const positionVAO = gl.createVertexArray();
    gl.bindVertexArray(positionVAO);
    gl.enableVertexAttribArray(0);
    gl.vertexAttribPointer(0, 2, gl.FLOAT, false, 0, 0);
    
    buffers['position'] = {
        "vbo": positionBuffer,
        "vao": positionVAO
    }
}


function drawGL() {
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.useProgram(shaders['lighting'].program);
    gl.bindVertexArray(buffers['position'].vao)
    gl.drawArrays(gl.TRIANGLES, 0, 6);
}


window.onload = initGL;
