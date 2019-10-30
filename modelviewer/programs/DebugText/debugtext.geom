#version 420

/** Geometry shader for rendering debug text.
 *  Expects a texture containing ASCII characters 0x20 to 0x7F, all the same
 *  dimensions, 16 columns x 6 rows.
 *  Takes two attribute buffer inputs:
 *  0: vec3, position to render (top left of) each character
 *  1: uint, bitfield xxrrggbb cccccccc
 *     xx: not used
 *     rr, gg, bb: character color
 *     cccccccc: ASCII character to render (must be in range 0x20-0x7F)
 *  Used with debugtext.vert.
 */

//we consume points and produce triangle strips with max 4
//(in fact, always 4) vertices.
layout(points) in;
layout(triangle_strip, max_vertices=4) out;

uniform sampler2D inTexture;

//transformation matrices
uniform mat4 matProjection; //projection matrix
uniform mat4 matModelview;  //modelview matrix

//render parameters set by host program (gpu/text.cpp)
//charBase must NOT be initialized to zero (which is the default)
//or else it will get optimized out of existence.
uniform int   charBase = 0x20; //ASCII value of first character in font
uniform ivec2 gridSize; //number of character (columns,rows) in the texture

//inputs from vertex shader
in DEBUGTEXT_CHAR {
	vec4 color; //character color
	uint chr;   //which character
} text_input[];

//outputs to fragment shader
out vec4 fragColor;     //fragment color
out vec2 fragTexCoord;  //fragment texture coordinates

//table of vertices to make a square
vec2 vtx_offsets[4] = vec2[](
	vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, 1)
);

void main() {
	ivec2 texSize  = textureSize(inTexture, 0);
	ivec2 charSize = texSize / gridSize;

	for(int i = 0; i < gl_in.length(); i++) { //for each primitive

		//compute this character's position in the grid
		uint chr = text_input[i].chr - charBase;
		vec2 texCoord = vec2(chr % gridSize.x, chr / gridSize.x);

		//render a square
		for(int j = 0; j < 4; j++) { //for each vertex

			//project the vertex position to pixel coordinates
			gl_Position = matProjection * matModelview * vec4(
			//gl_Position = vec4(
				gl_in[i].gl_Position.xy + (vtx_offsets[j].xy * charSize),
				gl_in[i].gl_Position.zw
			);

			//set the texture coords for this character
			fragTexCoord = ((texCoord + vtx_offsets[j]) * charSize) / texSize;
			fragColor = text_input[i].color;

			EmitVertex();
		}
		EndPrimitive();
	}
}
