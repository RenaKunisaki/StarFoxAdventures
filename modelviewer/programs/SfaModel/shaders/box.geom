#version 330
#extension GL_ARB_shading_language_420pack : require
#include "common.geom"
/* Geometry shader for boxes.
 */

layout(points) in;
//layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;
//layout(line_strip, max_vertices=256) out;

in VertexAttrib {
    vec3 vtxA, vtxB;
    vec4 color;
} vertex[];


void drawBox(vec3 vA, vec3 vB, vec4 color) {
    vec3 vtxs[8] = {
        vec3(vB.x, vB.y, vB.z),
        vec3(vA.x, vB.y, vB.z),
        vec3(vB.x, vB.y, vA.z),
        vec3(vA.x, vB.y, vA.z),
        vec3(vB.x, vA.y, vA.z),
        vec3(vA.x, vA.y, vA.z),
        vec3(vB.x, vA.y, vB.z),
        vec3(vA.x, vA.y, vB.z),
    };
    int faces[36] = {
    //  A  B  C   B  C  D
        0, 2, 4,  2, 4, 6, // -X
        1, 3, 5,  3, 5, 7, // +X
        0, 1, 2,  1, 2, 3, // -Y
        6, 7, 4,  7, 4, 5, // +Y
        0, 6, 1,  6, 1, 7, // -Z
        2, 4, 3,  4, 3, 5, // +Z
    };
    for(int i=0; i<36; i++)
        drawVtx(vtxs[faces[i]], color);
    EndPrimitive();
}

void main() {
    for(int i = 0; i < gl_in.length(); i++) { //for each primitive
        drawBox(vertex[i].vtxA, vertex[i].vtxB, vertex[i].color);
    }
    EndPrimitive();
}
