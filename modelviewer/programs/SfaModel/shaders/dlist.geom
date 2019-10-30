#version 330
#include "common.geom"
/* Geometry shader for display lists.
 */

layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;

in VertexAttrib {
    vec3 vtx;
    vec4 color;
} vertex[];

void drawQuad(vec3 a, vec3 b, vec3 c, vec3 d, vec4 col) {
    drawVtx(a, col);
    drawVtx(b, col);
    drawVtx(c, col);
    drawVtx(d, col);
    EndPrimitive();
}

void drawOutline(vec3 a, vec3 b, vec3 c) {
    //calculate face normal, and scale down to be almost flat.
    vec3 nrm = faceNormal(a, b, c) * 0.07;

    //if two vertices of a triangle are the same, it won't draw.
    //so to draw lines, we ensure they're not quite identical.
    //this also ensures the outline is at least partially visible
    //on both sides of the face.
    //we also use a quad to keep it consistent.
    vec3 a1 = a+nrm,   b1 = b+nrm,   c1 = c+nrm;
    vec3 a2 = a-nrm,   b2 = b-nrm,   c2 = c-nrm;
    vec4 col = vec4(0, 0, 0, 1);
    drawQuad(a1, b1, b2, a2, col);
    drawQuad(b1, c1, c2, b2, col);
    drawQuad(c1, a1, a2, c2, col);
}

void main() {
    for(int i = 0; i < gl_in.length(); i += 3) { //for each primitive
        for(int j=0; j<3; j++) {
            drawVtx(vertex[i+j].vtx, vertex[i+j].color);
        }
        EndPrimitive();
        drawOutline(vertex[i].vtx, vertex[i+1].vtx, vertex[i+2].vtx);
    }
}
