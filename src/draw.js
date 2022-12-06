import { vec4 } from 'gl-matrix';
import { VS_LIGHTING, FS_LIGHTING } from './shaders/lighting.js'
import { Cube } from './shapes/cube.js';
import { Camera } from './utils/camera.js';
import { initShaderProgram } from './utils/shaderloader.js'

/*

RESOURCE ZONE:
- Resizing: https://webgl2fundamentals.org/webgl/lessons/webgl-resizing-the-canvas.html

*/

let gl;
let canvas;
let camera; 

let squareVAO;
let squareVBO;

let cube;

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
            vertexPosition: gl.getAttribLocation(l_shader, "vertexPosition"),
            vertexNormal: gl.getAttribLocation(l_shader, "vertexNormal")
        },
        uniforms: {
            projMatrix: gl.getUniformLocation(l_shader, "projMatrix"),
            viewMatrix: gl.getUniformLocation(l_shader, "viewMatrix"),
        }
    }

    const positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    cube = new Cube(5).getVertices();
    const positions = cube;
      
    
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    const positionVAO = gl.createVertexArray();
    gl.bindVertexArray(positionVAO);
    gl.enableVertexAttribArray(0);
    gl.vertexAttribPointer(0, 3, gl.FLOAT, false, 6, 0);
    gl.enableVertexAttribArray(1);
    gl.vertexAttribPointer(1, 3, gl.FLOAT, false, 6, 3);
    gl.bindAttribLocation(l_shader, 0, "vertexPosition");
    gl.bindAttribLocation(l_shader, 1, "vertexNormal");

    buffers['position'] = {
        "vbo": positionBuffer,
        "vao": positionVAO
    }
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    gl.bindVertexArray(null);

    camera = new Camera(
        gl, 
        vec4.fromValues(0, 0, 0, 1), 
        vec4.fromValues(1, 0, 0, 0), 
        vec4.fromValues(0, 1, 0, 0),
        45, 0.1, 100, gl.canvas.width, gl.canvas.height
    )

    
}


function drawGL() {
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.useProgram(shaders['lighting'].program);
    
    gl.uniformMatrix4fv(shaders['lighting'].uniforms.viewMatrix, false, camera.projMatrix);
    gl.uniformMatrix4fv(shaders['lighting'].uniforms.projMatrix, false, camera.viewMatrix);
    
    gl.bindVertexArray(buffers['position'].vao)
    gl.drawArrays(gl.TRIANGLES, 0, cube.length/6);
    gl.bindVertexArray(null);
    gl.useProgram(null);

    // loop scene
    requestAnimationFrame(drawGL);
}

window.onload = () => {
    initGL();
    drawGL();
}
