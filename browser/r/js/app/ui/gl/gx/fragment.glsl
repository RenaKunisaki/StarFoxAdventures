/** Fragment shader for GX.
 */
varying lowp  vec4      vColor;    //vertex color
varying highp vec3      vLighting; //light color
varying highp vec2      vTexCoord; //texture coord
uniform       sampler2D uSampler0; //texture 0
uniform       sampler2D uSampler1; //texture 1
varying highp vec4      vId;       //ID for picker
uniform       bool      useId;     //are we rendering for picker?
uniform       bool      useLights; //enable lighting?
uniform       bool      useTexture; //enable textures?

void main() {
    if(useId) {
        //render for picker buffer. vertex color = its ID.
        gl_FragColor = vec4(
            vId.r / 255.0,
            vId.g / 255.0,
            vId.b / 255.0,
            vId.a / 255.0);
    }
    else { //render for display.
        highp vec4 tex0 = texture2D(uSampler0, vTexCoord);
        highp vec4 tex1 = texture2D(uSampler1, vTexCoord);
        highp vec4 col;
        if(useTexture) col = mix(tex0, tex1, (1.0-tex0.a) * tex1.a) * vColor;
        else col = vColor;
        if(useLights) col = vec4(col.rgb * vLighting, col.a);
        //XXX there's probably a shader flag that controls this.
        if(col.a <= 0.0) discard;
        else gl_FragColor = col.rgba;
    }
}
