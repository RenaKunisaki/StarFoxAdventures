#version 300 es
/** Fragment shader for GX.
 */
precision mediump float;
in      lowp  vec4 vColor;    //vertex color
in      highp vec3 vLighting; //light color
in      highp vec2 vTexCoord; //texture coord
flat in uvec4      vId;       //ID for picker
uniform sampler2D  uSampler0; //texture 0
uniform sampler2D  uSampler1; //texture 1
uniform bool       useId;     //are we rendering for picker?
uniform bool       useLights; //enable lighting?
uniform bool       useTexture; //enable textures?
out     vec4       outColor;

void main() {
    if(useId) {
        //render for picker buffer. vertex color = its ID.
        outColor = vec4(
            float(vId.r) / 255.0,
            float(vId.g) / 255.0,
            float(vId.b) / 255.0,
            float(vId.a) / 255.0);
    }
    else { //render for display.
        highp vec4 tex0 = texture(uSampler0, vTexCoord);
        highp vec4 tex1 = texture(uSampler1, vTexCoord);
        highp vec4 col;
        if(useTexture) col = mix(tex0, tex1, (1.0-tex0.a) * tex1.a) * vColor;
        else col = vColor;
        if(useLights) col = vec4(col.rgb * vLighting, col.a);
        //XXX there's probably a shader flag that controls this.
        if(col.a <= 0.0) discard;
        else outColor = col.rgba;
    }
}
