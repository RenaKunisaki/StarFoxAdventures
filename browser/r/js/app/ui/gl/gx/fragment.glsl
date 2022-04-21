#version 300 es
/** Fragment shader for GX.
 */
precision mediump float;

//inputs from previous stage
in vec4 vtx_Color;    //vertex color
in vec3 vtx_LightColor; //light color
in vec2 vtx_TexCoord; //texture coord
flat in uint  vtx_Id;       //ID for picker

//textures
uniform sampler2D u_texture0; //texture 0
uniform sampler2D u_texture1; //texture 1

//settings
uniform bool u_useId;        //are we rendering for picker?
uniform bool u_useLights;    //enable lighting?
uniform bool u_useTexture;   //enable textures?
uniform bool u_useAlphaTest; //discard texels at 0 alpha?

//outputs
out vec4 out_Color; //fragment color

void main() {
    vec4 tex0 = texture(u_texture0, vtx_TexCoord);
    vec4 tex1 = texture(u_texture1, vtx_TexCoord);
    vec4 col;
    if(u_useTexture) col = mix(tex0, tex1, (1.0-tex0.a) * tex1.a) * vtx_Color;
    else col = vtx_Color;
    if(u_useLights) col = vec4(col.rgb * vtx_LightColor, col.a);
    if(u_useAlphaTest && col.a <= 0.0) discard;
    else if(u_useId) {
        //render for picker buffer. vertex color = its ID.
        out_Color = vec4(
            float( int(vtx_Id) >> 24) / 255.0,
            //gl_FragCoord.y / 1024.0,
            float((int(vtx_Id) >> 16) & 0xFF) / 255.0,
            float((int(vtx_Id) >>  8) & 0xFF) / 255.0,
            float( int(vtx_Id)        & 0xFF) / 255.0);
    }
    else out_Color = col.rgba;
}
