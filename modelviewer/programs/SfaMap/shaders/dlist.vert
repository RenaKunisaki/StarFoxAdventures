#version 330
in vec3 vtx;
in vec2 texCoord;
in uint color;
in int polyIdx;

out VertexAttrib {
  vec3 vtx;
  vec2 texCoord;
  uint color;
  int  polyIdx;
} vertex;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    //gl_Position = vec4(vert, 1.0);
    //gl_Position = vec4(vert, 0, 1);
    vertex.vtx      = vtx;
    vertex.texCoord = texCoord;
    vertex.color    = color;
    vertex.polyIdx  = polyIdx;
    //fragColor = vtxColor;
}
