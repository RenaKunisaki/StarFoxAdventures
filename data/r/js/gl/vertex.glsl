//Example shader, no longer used.
attribute vec4 vtxPos;

uniform mat4 matModelView;
uniform mat4 matProjection;

void main() {
    gl_Position = matProjection * matModelView * vtxPos;
}
