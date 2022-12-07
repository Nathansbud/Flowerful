import { mat4, vec3, vec4 } from 'gl-matrix'

export class Camera {
    // Private fields
    #aspectRatio;
    #heightAngle;
    #widthAngle;

    #initialPosition;
    #initialLook;
    #initialUp;

    #position;
    #look;
    #up;
    
    #u;
    #v;
    #w;

    #near;
    #far;

    #scale;
    #unhinging;
    #projection;

    #viewMatrix;
    #worldMatrix;
    
    static #remap = mat4.fromValues(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -2, 0,
        0, 0, -1, 1
    );
    
    constructor(gl, position, look, up, heightAngle, near, far, width, height) {
        this.#aspectRatio = width / height;
        this.#heightAngle = heightAngle;
        this.#widthAngle = 2 * Math.atan(Math.tan(this.#heightAngle / 2) * this.#aspectRatio);
        
        this.#initialPosition = position;
        this.#initialLook = look;
        this.#initialUp = up;
    
        this.#position = position;
        this.#look = look;
        this.#up = up;

        this.#computeView();

        this.#near = near;
        this.#far = far;
        
        const c = -this.#near/this.#far;

        this.#scale = mat4.fromValues(
            1 / (far * Math.tan(this.#widthAngle / 2)), 0, 0, 0, 
            0, 1 / (far * Math.tan(this.#heightAngle / 2)), 0, 0,
            0, 0, 1 / far, 0,
            0, 0, 0, 1
        );

        this.#unhinging = mat4.fromValues(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1/(1 + c), -1,
            0, 0, -c/(1 + c), 0
        );

        this.#projection = mat4.create();
        mat4.multiply(
            this.#projection, 
            Camera.#remap, 
            mat4.multiply(mat4.create(), this.#unhinging, this.#scale)
        );
    }

    #computeView() {
        this.#w = vec4.normalize(vec4.create(), vec4.negate(vec4.create(), this.#look));
        this.#v = vec4.normalize(
            vec4.create(), 
            vec4.scaleAndAdd(vec4.create(), this.#up, this.#w, -vec4.dot(this.#up, this.#w))
        );

        this.#u = vec4.normalize(
            vec4.create(),
            vec3.cross(
                vec4.create(),
                this.#v,
                this.#w
            )
        );
        
        this.#viewMatrix = mat4.fromValues(
            this.#u[0], this.#v[0], this.#w[0], 0,
            this.#u[1], this.#v[1], this.#w[1], 0,
            this.#u[2], this.#v[2], this.#w[2], 0,
            -this.#position[0], -this.#position[1], -this.#position[2], 1
        );

        this.#worldMatrix = mat4.invert(mat4.create(), this.#viewMatrix);
    }

    reset() {
        this.#position = this.#initialPosition;
        this.#look = this.#initialLook;
        this.#up = this.#initialUp;
        this.computeView();
    }

    updateFrustum() { console.error("Camera updateFrustum not implemented yet!") }
    #rodriguez() { console.error("Camera rodriguez not implemented yet!") }
    rotate() { console.error("Camera rotate not implemented yet!") }
    move() { console.error("Camera move not implemented yet!") }

    get viewMatrix() { return this.#viewMatrix }
    get projMatrix() { return this.#projection }
    get worldPosition() { return vec4.transformMat4(vec4.create(), vec4.fromValues(0, 0, 0, 1), this.#worldMatrix) }
}
