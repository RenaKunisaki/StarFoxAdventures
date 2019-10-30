#version 400

/** Used with debugtext.geom.glsl. */

//input attributes: DebugText::Character
in ivec3 character; //ivec3(x, y, char_and_attrs)

//output attributes: converted color and character number
out DEBUGTEXT_CHAR {
	vec4 color; //character color
	uint chr;   //which character
} text_output;
//although we use GL_UNSIGNED_BYTE, it seems GLSL doesn't have such a type
//and the value gets automatically converted to uint.

//render parameters that probably won't get changed
uniform float textZpos;

void main() {
	gl_Position = vec4(character.xy, textZpos, 1);

	//expand the color byte into vec4
	text_output.color = vec4(
		float(character.z & 0x3000) / 0x3000, //00110000 00000000
		float(character.z & 0x0C00) / 0x0C00, //00001100 00000000
		float(character.z & 0x0300) / 0x0300, //00000011 00000000
        //1, 1, 1,
		1);
	text_output.chr = (character.z & 0xFF);
}
