#version 330
/* Geometry shader for bones.
 */
#include "common.geom"
layout(points) in;
layout(line_strip, max_vertices=256) out;

in VertexAttrib {
    vec3 head, tail;
    vec4 colHead, colTail;
} bone[];


void drawBone(vec3 head, vec3 tail, vec4 colHead, vec4 colTail) {
    /* vec3 vA = normalize(vec3(head.y, head.x, head.z));
    vec3 vB = normalize(vec3(tail.y, tail.x, tail.z));
    drawVtx(vec3(vB.x, vB.y, vB.z), color);
    drawVtx(vec3(vA.x, vB.y, vB.z), color);
    drawVtx(vec3(vB.x, vB.y, vA.z), color);
    drawVtx(vec3(vA.x, vB.y, vA.z), color);
    drawVtx(vec3(vB.x, vA.y, vA.z), color);
    drawVtx(vec3(vA.x, vA.y, vA.z), color);
    drawVtx(vec3(vB.x, vA.y, vB.z), color);
    drawVtx(vec3(vA.x, vA.y, vB.z), color);
    EndPrimitive(); */
    drawVtx(head, colHead);
    drawVtx(tail, colTail);
    EndPrimitive();
}


void main() {
    for(int i = 0; i < gl_in.length(); i++) { //for each primitive
        drawBone(bone[i].head, bone[i].tail, bone[i].colHead, bone[i].colTail);
    }
}
