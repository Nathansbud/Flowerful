import { vec3 } from 'gl-matrix'

export class Sphere {
    #param1;
    #param2;
    #verts;
    #buf;

    constructor(p1, p2) {
        this.#param1 = p1;
        this.#param2 = p2;

        this.#buf = vec3.create();

        this.#verts = []

        const angle = (2 * Math.PI) / Sphere.#param2;
        for(let i = 0; i < param2; i++) {
            this.#makeWedge(i * angle, (i + 1) * angle);
        }
    }

    #makeWedge(currentTheta, nextTheta) {
        const phiStep = (Math.PI / 2) / Sphere.#param1;
        for(let i = 0; i < Sphere.#param1; i++) {
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
        this.#insertVec3(vec3.normalize(this.#buf, topLeft));
        
        this.#insertVec3(bottomLeft);
        this.#insertVec3(vec3.normalize(this.#buf, bottomLeft));

        this.#insertVec3(bottomRight);
        this.#insertVec3(vec3.normalize(this.#buf, bottomRight));

        this.#insertVec3(topLeft);
        this.#insertVec3(vec3.normalize(this.#buf, topLeft));
        
        this.#insertVec3(bottomRight);
        this.#insertVec3(vec3.normalize(this.#buf, bottomRight));

        this.#insertVec3(topRight);
        this.#insertVec3(vec3.normalize(this.#buf, topRight));
    }

    #insertVec3(v) {
        this.#verts.push(v.x, v.y, v.z);
    }
}
