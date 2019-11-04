#version 330
/** Geometry shader for displaying texture coordinates.
 */

layout(triangles) in;
layout(triangle_strip, max_vertices=4) out;
uniform sampler2D texture;

//inputs from vertex shader
in VertexAttrib {
    vec3 texCoord; //x, y, useTexture
    vec4 color;
} vertex[];

uniform mat4 matProjection; //projection matrix

out vec4 fragColor;
out vec3 fragTexCoord; //x, y, useTexture


void drawVtx(vec2 pos, vec4 color, vec3 texCoord) {
    vec4 p = vec4(pos.xy, 0, 1);
    gl_Position  = matProjection * p;
    fragColor    = color;
    fragTexCoord = texCoord;
    EmitVertex();
}

void main() {
    ivec2 texSize = textureSize(texture, 0);
    for(int i = 0; i < gl_in.length(); i += 3) { //for each primitive
        for(int j=0; j<3; j++) {
            drawVtx(
                vertex[i+j].texCoord.xy * texSize,
                vertex[i+j].color, vertex[i+j].texCoord);
        }
        EndPrimitive();
    }
}
