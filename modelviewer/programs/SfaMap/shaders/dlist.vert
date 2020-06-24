#version 330
in vec3 vtx;
in vec2 texCoord;
in uint color;
in vec3 offset;
//uniform sampler1D positions;

out VertexAttrib {
  vec3 vtx;
  vec2 texCoord;
  uint color;
} vertex;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    vertex.vtx      = vtx + offset;
    vertex.texCoord = texCoord;
    vertex.color    = color;
    //fragColor = vtxColor;
    //if you so much as mention gl_InstanceID, the model
    //just plain won't render. this is probably Yet Another Driver Bug.
}
