#version 400

/** Vertex shader for displaying texture coordinates.
 */

//input attributes: texture coord, point color
in vec3 texCoord; //x, y, useTexture
in vec4 color;

out VertexAttrib {
    vec3 texCoord;
    vec4 color;
} vertex;

void main() {
	vertex.texCoord = texCoord;
    vertex.color = color;
}
