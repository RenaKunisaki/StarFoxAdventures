#version 330
in vec3 head;
in vec3 tail;
in vec4 colHead;
in vec4 colTail;

out VertexAttrib {
  vec3 head, tail;
  vec4 colHead, colTail;
} bone;

//out vec4 fragColor;
//out vec2 fragTexCoord;

void main() {
    //gl_Position = vec4(head, 1.0);
    //gl_Position = vec4(vert, 0, 1);
    bone.head  = head;
    bone.tail  = tail;
    bone.colHead = colHead;
    bone.colTail = colTail;
    //fragColor  = color;
}
