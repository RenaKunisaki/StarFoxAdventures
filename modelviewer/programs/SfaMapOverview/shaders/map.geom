#version 330
#extension GL_ARB_shading_language_420pack : require
#include "common.geom"
/* Geometry shader for maps.
 */

layout(points) in;
//layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;
//layout(line_strip, max_vertices=256) out;

in VertexAttrib {
    vec2 corner1, corner2;
    vec4 color;
    float z;
} vertex[];


void drawBox(vec2 vA, vec2 vB, vec4 color, float z) {
    vec3 vtxs[4] = {
        vec3(vA.x, vA.y, z),
        vec3(vB.x, vA.y, z),
        vec3(vB.x, vB.y, z),
        vec3(vA.x, vB.y, z),
    };
    int faces[6] = {
        // 0--1
        // | /|
        // |/ |
        // 3--2
        0, 1, 3,  1, 2, 3,
    };
    for(int i=0; i<6; i++)
        drawVtx(vtxs[faces[i]], color);
    EndPrimitive();
}


void main() {
    for(int i = 0; i < gl_in.length(); i++) { //for each primitive
        drawBox(vertex[i].corner1, vertex[i].corner2, vertex[i].color,
            vertex[i].z);
    }
    EndPrimitive();
}
