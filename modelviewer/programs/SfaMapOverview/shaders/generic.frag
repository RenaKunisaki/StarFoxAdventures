#version 330
in      vec4      fragColor;   //set by vertex shader
in      vec2      fragTexCoord;
out     vec4      outputColor; //the resulting fragment color
uniform bool      enableTexture = false;
uniform sampler2D texture;
uniform float     minAlpha      = 0.0;  //discard texels where alpha < minAlpha

void main() {
    vec4 color = fragColor;
	if(enableTexture) {
		color *= texture2D(texture, fragTexCoord.st).rgba;
	}
	if(color.a < minAlpha) discard;
    //gl_FragDepth = gl_FragCoord.z;
	outputColor = color;
}
