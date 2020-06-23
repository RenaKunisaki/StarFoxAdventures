#version 330
in vec2 corner1;
in vec2 corner2;
in vec4 color;
in float z;

out VertexAttrib {
  vec2 corner1, corner2;
  vec4 color;
  float z;
} vertex;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    //gl_Position = vec4(vertA, 1.0);
    //gl_Position = vec4(vert, 0, 1);
    vertex.corner1 = corner1;
    vertex.corner2 = corner2;
    vertex.color   = color;
    vertex.z       = z;
    //fragColor = vtxColor;
}
