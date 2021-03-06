#version 400

//automatic variables:
//in vec4 gl_FragCoord; //The location of the fragment in window space
//in bool gl_FrontFacing; //true if this fragment was generated by the front-face of the primitive
//in vec2 gl_PointCoord; //location within a point primitive that defines the position of the fragment relative to the side of the point.
//out float gl_FragDepth; //the fragment's depth. If the shader does not statically write this value, then it will take the value of gl_FragCoord.z​.

//GLSL 4.0+:
//out int gl_SampleMask[]; //sample mask for the fragment when performing mutlisampled rendering

//automatic variables (GL 4.0+):
//in int gl_SampleID; //an integer identifier for the current sample that this fragment is rasterized for.
	//Warning: Any use of this variable at all will force this shader to be evaluated per-sample. Since much of the point of multisampling is to avoid that, you should use it only when you must.
//in vec2 gl_SamplePosition; //the location of the current sample for the fragment within the pixel's area, with values on the range [0, 1]. The origin is the bottom-left of the pixel area.
	//Warning: Any use of this variable at all will force this shader to be evaluated per-sample. Since much of the point of multisampling is to avoid that, you should use it only when you must.
//in int gl_SampleMaskIn[]; //When using multisampling, this variable contains a bitfield for the sample mask of the fragment being generated. The array is as long as needed to fill in the number of samples supported by the GL implementation.

//automatic, overrideable variables:
//in float gl_ClipDistance[]; //the interpolated clipping plane half-spaces, as output for vertices from the last Vertex Processing stage.
//in int gl_PrimitiveID; //the index of the current primitive being rendered by this drawing command.

//GL 4.3+:
//in int gl_Layer; //the layer number for this primitive output by the Geometry Shader
//in int gl_ViewportIndex; //the viewport index for this primitive output by the Geometry Shader


/*
// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

    // Output color = color of the texture at the specified UV
    //color = texture( myTextureSampler, UV ).rgb;
}
*/

uniform bool      enableTexture = false;
uniform float     minAlpha      = 0.0;  //discard texels where alpha < minAlpha
uniform vec4      modColor      = vec4(1,1,1,1); //multiply all colors by this
uniform sampler2D inTexture;
in      vec4      fragColor;   //set by vertex shader
in      vec2      fragTexCoord;
out     vec4      outputColor; //the resulting fragment color

void main () {
	vec4 color = fragColor;
	if(enableTexture) {
		color *= texture2D(inTexture, fragTexCoord.st).rgba;
	}
	color *= modColor;
	if(color.a < minAlpha) discard;
    //gl_FragDepth = gl_FragCoord.z;
	outputColor = color;
    //outputColor = vec4(
    //    gl_FragCoord.x / 1000,
    //    gl_FragCoord.y / 1000,
    //    gl_FragCoord.z * 1000,
    //    1);
    //outputColor = vec4(1, 0, 0, 1);
}
