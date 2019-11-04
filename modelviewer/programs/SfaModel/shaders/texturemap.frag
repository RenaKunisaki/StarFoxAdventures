#version 330
/** Fragment shader for displaying texture coordinates.
 */
in      vec4      fragColor;   //set by vertex shader
in      vec3      fragTexCoord; //x, y, useTexture
out     vec4      outputColor; //the resulting fragment color
uniform sampler2D texture;
uniform float     minAlpha = 0.0;  //discard texels where alpha < minAlpha

void main() {
    vec4 color = fragColor;
	if(fragTexCoord.z > 0.5) {
		color *= texture2D(texture, fragTexCoord.xy).rgba;
	}
	if(color.a < minAlpha) discard;
    //gl_FragDepth = gl_FragCoord.z;
	outputColor = color;
}
