#version 330
#include "common.geom"
/* Geometry shader for display lists.
 */

layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;
uniform sampler2D texture;

in VertexAttrib {
    vec3 vtx;
    vec2 texCoord;
    uint color;
} vertex[];

vec4 unpackColor(uint color) {
    float r = float(int(color>> 0) & 255) / 255.0;
    float g = float(int(color>> 8) & 255) / 255.0;
    float b = float(int(color>>16) & 255) / 255.0;
    float a = float(int(color>>24) & 255) / 255.0;
    return vec4(r, g, b, a);
}

void drawQuad(vec3 a, vec3 b, vec3 c, vec3 d, vec4 col) {
    drawVtx(a, col);
    drawVtx(b, col);
    drawVtx(c, col);
    drawVtx(d, col);
    EndPrimitive();
}

void main() {
    //ivec2 texSize  = textureSize(texture, 0);
    for(int i = 0; i < gl_in.length(); i += 3) { //for each primitive
        for(int j=0; j<3; j++) {
            vec3 pos = vertex[i+j].vtx;
            vec4 color = unpackColor(vertex[i+j].color);
            drawVtx(pos, color, vertex[i+j].texCoord);
        }
        EndPrimitive();
    }
}
