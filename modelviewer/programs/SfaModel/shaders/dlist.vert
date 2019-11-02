#version 330
in vec3 vert;
in vec2 texCoord;
in vec4 vtxColor;

out VertexAttrib {
  vec3 vtx;
  vec2 texCoord;
  vec4 color;
} vertex;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    //gl_Position = vec4(vert, 1.0);
    //gl_Position = vec4(vert, 0, 1);
    vertex.vtx      = vert;
    vertex.texCoord = texCoord;
    vertex.color    = vtxColor;
    //fragColor = vtxColor;
}
