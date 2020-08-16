#version 330
#include "common.geom"
/* Geometry shader for display lists.
 */

layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;
uniform sampler2D texture;
uniform bool enableOutline = true; //draw the outlines?
uniform bool enableFill    = true; //draw the actual polygons?
uniform int  selectedPoly  = -1; //which poly to fill (-1=all)
uniform float lineAlpha    = 1.0; //outline alpha

in VertexAttrib {
    vec3 vtx;
    vec2 texCoord;
    uint color;
    int  polyIdx;
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

void drawOutline(vec3 a, vec3 b, vec3 c) {
    //calculate face normal, and scale down to be almost flat.
    vec3 nrm = faceNormal(a, b, c) * 0.02;

    //if two vertices of a triangle are the same, it won't draw.
    //so to draw lines, we ensure they're not quite identical.
    //this also ensures the outline is at least partially visible
    //on both sides of the face.
    //we also use a quad to keep it consistent.
    vec3 a1 = a+nrm,   b1 = b+nrm,   c1 = c+nrm;
    vec3 a2 = a-nrm,   b2 = b-nrm,   c2 = c-nrm;
    vec4 col = vec4(0, 0, 0, lineAlpha);
    drawQuad(a1, b1, b2, a2, col);
    drawQuad(b1, c1, c2, b2, col);
    drawQuad(c1, a1, a2, c2, col);
}

void main() {
    //ivec2 texSize  = textureSize(texture, 0);
    for(int i = 0; i < gl_in.length(); i += 3) { //for each primitive
        int polyIdx = vertex[i].polyIdx;
        if(enableFill
        && (polyIdx == selectedPoly || selectedPoly == -1)) {
            for(int j=0; j<3; j++) {
                vec3 pos = vertex[i+j].vtx;
                vec4 color = unpackColor(vertex[i+j].color);
                drawVtx(pos, color, vertex[i+j].texCoord);
            }
            EndPrimitive();
        }
        if(enableOutline) drawOutline(
            vertex[i].vtx, vertex[i+1].vtx, vertex[i+2].vtx);
    }
}
