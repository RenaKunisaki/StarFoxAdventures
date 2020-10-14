/** Fragment shader for GX.
 */
varying lowp  vec4      vColor;    //vertex color
varying highp vec3      vLighting; //light color
varying highp vec2      vTexCoord; //texture coord
uniform       sampler2D uSampler;  //texture
varying highp vec4      vId;       //ID for picker
uniform       bool      useId;     //are we rendering for picker?

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
        //without texture:
        //highp vec3 color = vColor.rgb * vLighting;
        //gl_FragColor = vec4(color.r, color.g, color.b, vColor.a);

        //with texture:
        highp vec4 texelColor = texture2D(uSampler, vTexCoord) * vColor;
        gl_FragColor = vec4(texelColor.rgb * vLighting, texelColor.a);
        //gl_FragColor = vec4(texelColor.rgb * vLighting, 1);
        //gl_FragColor = vec4(texelColor.rgb * vLighting, vColor.a);
    }
}
