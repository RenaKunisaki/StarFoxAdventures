#version 330
in      vec4      fragColor;   //set by vertex shader
in      vec2      fragTexCoord;
out     vec4      outputColor; //the resulting fragment color

void main() {
    //outputColor = vec4(0.3, 0.5, 1.0, 1.0);
    outputColor = fragColor;
}
