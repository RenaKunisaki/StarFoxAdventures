#version 330
in vec3 head;
in vec3 tail;
in vec4 colHead;
in vec4 colTail;

out VertexAttrib {
  vec3 head, tail;
  vec4 colHead, colTail;
} line;

void main() {
    line.head    = head;
    line.tail    = tail;
    line.colHead = colHead;
    line.colTail = colTail;
}
