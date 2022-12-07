import { vec3 } from 'gl-matrix'

const buf = vec3.create();

export class Sphere {
    #param1;
    #param2;
    #data;

    constructor(p1, p2) {
        this.#param1 = p1;
        this.#param2 = p2;
        this.#data = []

        const angle = (2 * Math.PI) / this.#param2;
        for(let i = 0; i < this.#param2; i++) {
            this.#makeWedge(i * angle, (i + 1) * angle);
        }
    }

    getData() { return this.#data; }

    #makeWedge(currentTheta, nextTheta) {
        const phiStep = (Math.PI / 2) / this.#param1;
        for(let i = 0; i < this.#param1; i++) {
            const nextPhi = i * phiStep;
            const currentPhi = (i + 1) * phiStep;

            const topLeft = vec3.fromValues(
                0.5 * Math.sin(currentPhi) * Math.cos(currentTheta),
                0.5 * Math.cos(currentPhi),
                0.5 * Math.sin(currentTheta) * Math,sin(currentPhi)
            );

            const topRight = vec3.fromValues(
                0.5 * Math.sin(currentPhi) * Math.cos(nextTheta),
                0.5 * Math.cos(currentPhi),
                0.5 * Math.sin(nextTheta) * Math.sin(currentPhi)
            )

            const bottomLeft = vec3.fromValues(
                0.5 * Math.sin(nextPhi) * Math.cos(currentTheta),
                0.5 * Math.cos(nextPhi),
                0.5 * Math.sin(currentTheta) * Math.sin(nextPhi)
            )

            const bottomRight = vec3.fromValues(
                0.5 * Math.sin(nextPhi) * Math.cos(nextTheta),
                0.5 * Math.cos(nextPhi),
                0.5 * Math.sin(nextTheta) * Math.sin(nextPhi)
            )

            this.#makeTile(topLeft, topRight, bottomLeft, bottomRight);
        }
    }

    #makeTile(topLeft, topRight, bottomLeft, bottomRight) {
        this.#insertVec3(topLeft);
        this.#insertVec3(vec3.normalize(buf, topLeft));
        
        this.#insertVec3(bottomLeft);
        this.#insertVec3(vec3.normalize(buf, bottomLeft));

        this.#insertVec3(bottomRight);
        this.#insertVec3(vec3.normalize(buf, bottomRight));

        this.#insertVec3(topLeft);
        this.#insertVec3(vec3.normalize(buf, topLeft));
        
        this.#insertVec3(bottomRight);
        this.#insertVec3(vec3.normalize(buf, bottomRight));

        this.#insertVec3(topRight);
        this.#insertVec3(vec3.normalize(buf, topRight));
    }

    #insertVec3(v) {
        this.#data.push(v[0], v[1], v[2]);
    }
}

export class Cube {
    #param1;
    #param2;
    #data;

    constructor(p1, p2) {
        this.#param1 = p1;
        this.#param2 = p2;
        this.#data = [];
        this.#makeCube();
    }
    
    getData() { return this.#data; }
    
    #makeTile(topLeft, topRight, bottomLeft, bottomRight) {
        const nc = (a, b, c) => vec3.normalize(
            buf, 
            vec3.cross(buf, 
                vec3.subtract(buf, b, a),
                vec3.subtract(buf, c, a)
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
            buf,
            vec3.scaleAndAdd(
                buf, 
                topLeft, 
                vec3.subtract(buf, topRight, topLeft),
                f1
            ),
            vec3.subtract(buf, bottomLeft, topLeft),
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
        this.#data.push(v[0], v[1], v[2]);
    }
}

export class Cylinder {
    #param1; 
    #param2;
    #data;
    
    constructor(p1, p2) {
        this.#param1 = p1;
        this.#param2 = p2;
        this.#data = []
        this.#makeCylinder();    
    }

    #makeTile(topLeft, topRight, bottomLeft, bottomRight) {
        const zeroY = vec3.fromValues(1, 0, 1);
        
        const tln = vec3.normalize(buf, vec3.mul(buf, topLeft, zeroY));
        const brn = vec3.normalize(buf, vec3.mul(buf, bottomRight, zeroY));
        
        this.#insertVec3(topLeft);
        this.#insertVec3(tln);

        this.#insertVec3(bottomLeft);
        this.#insertVec3(vec3.normalize(buf, vec3.mul(buf, bottomLeft, zeroY)));

        this.#insertVec3(bottomRight);
        this.#insertVec3(brn);

        this.#insertVec3(topLeft);
        this.#insertVec3(tln);

        this.#insertVec3(bottomRight);
        this.#insertVec3(brn);

        this.#insertVec3(topRight);
        this.#insertVec3(vec3.normalize(buf, vec3.mul(buf, topRight, zeroY)));
    }

    #makeCapPiece(leftCurr, leftNext, rightCurr, rightNext, isTop) {
        const norm = vec3.fromValues(0, isTop ? 1 : -1, 0);

        this.#insertVec3(leftNext)
        this.#insertVec3(norm);

        this.#insertVec3(leftCurr)
        this.#insertVec3(norm);

        this.#insertVec3(rightNext);
        this.#insertVec3(norm);

        this.#insertVec3(leftCurr);
        this.#insertVec3(norm);

        this.#insertVec3(rightCurr);
        this.#insertVec3(norm);

        this.#insertVec3(rightNext);
        this.#insertVec3(norm);
    }

    #makeWall(currentTheta, nextTheta) {
        const onlyY = vec3.fromValues(0, 1, 0);
        const onlyNY = vec3.fromValues(0, -1, 0);

        const left = vec3.fromValues(0.5 * Math.sin(currentTheta), 0.5, 0.5 * Math.cos(currentTheta));
        const right = vec3.fromValues(0.5 * Math.sin(nextTheta), 0.5, 0.5 * Math.cos(nextTheta));

        for(let i = 0; i < this.#param1; i++) {
            const topStep = i / this.#param1;
            const nextStep = (i + 1) / this.#param1;

            this.#makeTile(
                vec3.scaleAndAdd(buf, left, onlyY, -topStep),
                vec3.scaleAndAdd(buf, right, onlyY, -topStep),
                vec3.scaleAndAdd(buf, left, onlyY, -nextStep),
                vec3.scaleAndAdd(buf, right, onlyY, -nextStep)
            )
            
            const leftPath = vec3.fromValues(-left[0], 0.5 - left[1], -left[2]);
            const rightPath = vec3.fromValues(-right[0], 0.5 - right[1], -right[2]);

            this.#makeCapPiece(
                vec3.scaleAndAdd(buf, left, leftPath, nextStep),
                vec3.scaleAndAdd(buf, right, rightPath, nextStep),
                vec3.scaleAndAdd(buf, left, leftPath, topStep),
                vec3.scaleAndAdd(buf, right, rightPath, topStep),
                true
            );
            
            leftPath[1] *= -1;
            rightPath[1] *= -1;
            left[1] *= -1;
            right[1] *= -1;

            this.#makeCapPiece(
                vec3.scaleAndAdd(buf, left, leftPath, topStep),
                vec3.scaleAndAdd(buf, right, rightPath, topStep),
                vec3.scaleAndAdd(buf, left, leftPath, nextStep),
                vec3.scaleAndAdd(buf, right, rightPath, nextStep),
                false
            );

            leftPath[1] *= -1;
            rightPath[1] *= -1;
            left[1] *= -1;
            right[1] *= -1;
        }
    }

    #makeCylinder() { 
        const thetaStep = Math.PI / this.#param2;
        for(let i = 0; i < this.#param2; i++) {
            const currentTheta = i * thetaStep;
            const nextTheta = (i + i) * thetaStep;
            this.#makeWall(currentTheta, nextTheta);
        }
    
    }

    #insertVec3(v) {
        this.#data.push(v[0], v[1], v[2]);
    }
}

export class Cone {
    constructor() {
        console.error("Unimplemented!")
    }
}
