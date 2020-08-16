#version 330
in vec3  pos;
in float radius;
in vec4  color;

out VertexAttrib {
  vec3  pos;
  float radius;
  vec4  color;
} vertex;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    //gl_Position = vec4(vertA, 1.0);
    //gl_Position = vec4(vert, 0, 1);
    vertex.pos    = pos;
    vertex.radius = radius;
    vertex.color  = color;
    //fragColor = vtxColor;
}
