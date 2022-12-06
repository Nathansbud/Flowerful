import { vec3, vec4 } from 'gl-matrix'

export class Cube {
    #param1;
    #param2;
    #verts;
    #buf;

    constructor(p1, p2) {
        this.#param1 = p1;
        this.#param2 = p2;
        this.#verts = [];
        
        this.#buf = vec3.create();

        this.#makeCube();
    }
    
    getVertices() {
        return this.verts;
    }
    
    #makeTile(topLeft, topRight, bottomLeft, bottomRight) {
        const nc = (a, b, c) => vec3.normalize(
            this.#buf, 
            vec3.cross(this.#buf, 
                vec3.subtract(this.#buf, b, a),
                vec3.subtract(this.#buf, c, a)
            )
        )
            
        this.#insertVec3(topLeft);
        this.#insertVec3(nc(topLeft, bottomLeft, bottomRight));
        
        this.#insertVec3(bottomLeft);
        this.#insertVec3(nc(bottomLeft, bottomRight, topLeft));

        this.#insertVec3(bottomRight);
        this.#insertVec3(nc(bottomRight, topLeft, bottomLeft));
        
        this.#insertVec3(topLeft);
        this.#insertVec3(nc(topLeft, bottomRight, topRight));

        this.#insertVec3(bottomRight);
        this.#insertVec3(nc(bottomRight, topRight, topLeft));
        
        this.#insertVec3(topRight);
        this.#insertVec3(nc(topRight, topLeft, bottomRight));
    }

    #makeFace(topLeft, topRight, bottomLeft, _) {
        const cv = (f1, f2) => vec3.scaleAndAdd(
            this.#buf,
            vec3.scaleAndAdd(
                this.#buf, 
                topLeft, 
                vec3.subtract(this.#buf, topRight, topLeft),
                f1
            ),
            vec3.subtract(this.#buf, bottomLeft, topLeft),
            f2
        )

        for(let i = 0; i < this.#param1; i++) {
            for(let j = 0; j < this.#param1; j++) {
                this.#makeTile(
                    cv(i / this.#param1, j / this.#param1),
                    cv((i + 1) / this.#param1, j / this.#param1),
                    cv(i / this.#param1, (j + 1) / this.#param1),
                    cv((i + 1) / this.#param1, (j + 1) / this.#param1)
                )
            }
        }
    }

    #makeCube() {
        const ppp = vec3.fromValues(0.5, 0.5, 0.5);
        const ppn = vec3.fromValues(0.5, 0.5, -0.5);
        const pnp = vec3.fromValues(0.5, -0.5, 0.5);
        const pnn = vec3.fromValues(0.5, -0.5, -0.5);
        const npp = vec3.fromValues(-0.5, 0.5, 0.5);
        const npn = vec3.fromValues(-0.5, 0.5, -0.5);
        const nnp = vec3.fromValues(-0.5, -0.5, 0.5);
        const nnn = vec3.fromValues(-0.5, -0.5, -0.5);

        this.#makeFace(npp, ppp, nnp, pnp);
        this.#makeFace(nnn, pnn, npn, ppn);
        this.#makeFace(npn, ppn, npp, ppp);
        this.#makeFace(nnp, pnp, nnn, pnn);
        this.#makeFace(pnp, ppp, pnn, ppn);
        this.#makeFace(nnn, npn, nnp, npp);
    }

    #insertVec3(v) {
        this.verts.push(v[0], v[1], v[2]);
    }
}
