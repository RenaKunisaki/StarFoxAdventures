#version 400

/** Vertex shader for displaying texture coordinates.
 */

//input attributes: texture coord, point color
in vec3 texCoord; //x, y, useTexture
in vec4 color;

out gl_PerVertex {
    //not used but must exist
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

out VertexAttrib {
    vec3 texCoord;
    vec4 color;
} vertex;

void main() {
	vertex.texCoord = texCoord;
    vertex.color = color;
}
