#version 330
#extension GL_ARB_shading_language_420pack : require
#include "common.geom"
/* Geometry shader for spheres.
 */

layout(points) in;
//layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;
//layout(line_strip, max_vertices=256) out;

in VertexAttrib {
    vec3  pos;
    float radius;
    vec4  color;
} vertex[];

void drawSphere(vec3 pos, float radius, vec4 color) {
    //https://community.khronos.org/t/sphere-generation/49590/2
    vec3 vtxs[64];
    int nStacks=8, nSlices=8, i=0;
    for(int stack = 0; stack < nStacks; ++stack) {
        for(int slice = 0; slice < nSlices; ++slice) {
            float y = 2.0 * stack / nStacks - 1.0;
            /* for better distribution, use y = -cos(PI * stack / nStacks) */
            float r = sqrt(1 - (y * y));
            float x = r * sin(2.0 * PI * slice / nSlices);
            float z = r * cos(2.0 * PI * slice / nSlices);
            drawVtx((radius * vec3(x, y, z)) + pos, color);
            vtxs[i++] = (radius * vec3(x, y, z)) + pos;
        }
    }

    //triangulate
    for(int stack = 1; stack < nStacks; ++stack) {
        for(int slice = 1; slice < nSlices; ++slice) {
            int v1 = (stack * nSlices) + slice;
            int v2 = v1 - 1;
            int v3 = v1 - nSlices;
            int v4 = v3 - 1;
            drawVtx(vtxs[v1], color);
            drawVtx(vtxs[v3], color); //note order here: 1 3 2 4
            drawVtx(vtxs[v2], color);
            drawVtx(vtxs[v4], color);
        }
        EndPrimitive();
    }
}


void main() {
    for(int i = 0; i < gl_in.length(); i++) { //for each primitive
        drawSphere(vertex[i].pos, vertex[i].radius, vertex[i].color);
    }
}
