import { mat4, vec3, vec4 } from 'gl-matrix'

export class Cube {

    constructor(p1, p2) {
        this.#param1 = p1;
        this.#verts = [];
        this.#makeCube();
    }
    
    getVertices() {
        return this.verts;
    }

    #makeTile(topLeft, topRight, bottomLeft, bottomRight) {
        insertVec3(m_vertexData, topLeft);
        insertVec3(m_vertexData, vec4.normalize(vec4.create(), 
            vec4.cross(vec4.create(), bottomLeft - topLeft, bottomRight - topLeft)));
        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, vec4.normalize(vec4.create(), 
            vec4.cross(vec4.create(), bottomRight - bottomLeft, topLeft - bottomLeft)));
        insertVec3(m_vertexData, bottomRight);
        insertVec3(m_vertexData, vec4.normalize(vec4.create(), 
            vec4.cross(vec4.create(), topLeft - bottomRight, bottomLeft - bottomRight)));
        insertVec3(m_vertexData, topLeft);
        insertVec3(m_vertexData, vec4.normalize(vec4.create(), 
            vec4.cross(vec4.create(), bottomRight - topLeft, topRight - topLeft)));
        insertVec3(m_vertexData, bottomRight);
        insertVec3(m_vertexData, vec4.normalize(vec4.create(), 
            vec4.cross(vec4.create(), topRight - bottomRight, topLeft - bottomRight)));
        insertVec3(m_vertexData, topRight);
        insertVec3(m_vertexData, vec4.normalize(vec4.create(), 
            vec4.cross(vec4.create(), topLeft - topRight, bottomRight - topRight)));

    }

    #makeFace(topLeft, topRight, bottomLeft) {
        for(let i = 0; i < param1; i++) {
            for(let j = 0; j < m_param1; j++) {
                makeTile(topLeft + ((topRight-topLeft)*float(i))/fparam + ((bottomLeft-topLeft)*float(j))/fparam,
                        topLeft + ((topRight-topLeft)*float(i+1))/fparam + ((bottomLeft-topLeft)*float(j))/fparam,
                        topLeft + ((topRight-topLeft)*float(i))/fparam + ((bottomLeft-topLeft)*float(j+1))/fparam,
                        topLeft + ((topRight-topLeft)*float(i+1))/fparam + ((bottomLeft-topLeft)*float(j+1))/fparam);
            }
        }
    }



    #makeCube() {
        makeFace(vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5));

        makeFace(vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5));

        makeFace(vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5));

        makeFace(vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5));

        makeFace(vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5));

        makeFace(vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5),
                vec3.fromValues(-0.5, 0.5, 0.5));
    }

    #insertVec3(v) {
        this.verts.push(v[0], v[1], v[2]);
    }
}