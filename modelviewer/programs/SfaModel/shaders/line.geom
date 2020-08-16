#version 330
/* Geometry shader for lines.
 */
#include "common.geom"
layout(points) in;
layout(line_strip, max_vertices=256) out;

in VertexAttrib {
    vec3 head, tail;
    vec4 colHead, colTail;
} line[];


void drawLine(vec3 head, vec3 tail, vec4 colHead, vec4 colTail) {
    drawVtx(head, colHead);
    drawVtx(tail, colTail);
    EndPrimitive();
}


void main() {
    for(int i = 0; i < gl_in.length(); i++) { //for each primitive
        drawLine(line[i].head, line[i].tail, line[i].colHead, line[i].colTail);
    }
}
