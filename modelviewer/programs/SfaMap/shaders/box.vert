#version 330
in vec3 vertA;
in vec3 vertB;
in vec4 vtxColor;

out VertexAttrib {
  vec3 vtxA, vtxB;
  vec4 color;
} vertex;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    //gl_Position = vec4(vertA, 1.0);
    //gl_Position = vec4(vert, 0, 1);
    vertex.vtxA  = vertA;
    vertex.vtxB  = vertB;
    vertex.color = vtxColor;
    //fragColor = vtxColor;
}
