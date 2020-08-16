#version 330
//this doesn't actually work because lol
in      vec4      fragColor;   //set by vertex shader
in      vec2      fragTexCoord;
out     vec4      outputColor; //the resulting fragment color
uniform bool      enableTexture = false;
uniform sampler2D texture;
uniform float     minAlpha      = 0.0;  //discard texels where alpha < minAlpha

float Ns = 250;
vec4 mat_specular=vec4(1);
vec4 light_specular=vec4(1);
vec3 lightDir=vec3(1);

void main() {
    vec4 color = fragColor;
	if(enableTexture) {
		color *= texture2D(texture, fragTexCoord.st).rgba;
	}
	if(color.a < minAlpha) discard;
    //gl_FragDepth = gl_FragCoord.z;
	//outputColor = color;


    // calculate normal from texture coordinates
    vec3 N;
    N.xy = gl_PointCoord * 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    // calculate lighting
    float diffuse = max(0.0, dot(lightDir, N));

    vec3 eye = vec3 (0.0, 0.0, 1.0);
    vec3 halfVector = normalize( eye + lightDir);
    float spec = max( pow(dot(N,halfVector), Ns), 0.);
    vec4 S = light_specular*mat_specular* spec;
    outputColor = color * diffuse + S;
}
