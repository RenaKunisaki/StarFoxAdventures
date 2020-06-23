#include "math.glsl"

uniform mat4 matProjection; //projection matrix
uniform mat4 matModelview;  //modelview matrix

out vec4 fragColor;
out vec2 fragTexCoord;


void drawVtx(vec4 pos, vec4 color, vec2 texCoord) {
    gl_Position  = (matProjection * matModelview) * pos;
    fragColor    = color;
    fragTexCoord = texCoord;
    EmitVertex();
}

void drawVtx(vec4 pos, vec4 color) {
    gl_Position = (matProjection * matModelview) * pos;
    fragColor   = color;
    EmitVertex();
}


void drawVtx(vec3 pos, vec4 color, vec2 texCoord) {
    drawVtx(vec4(pos.xyz, 1), color, texCoord);
}

void drawVtx(vec3 pos, vec4 color) {
    drawVtx(vec4(pos.xyz, 1), color);
}
