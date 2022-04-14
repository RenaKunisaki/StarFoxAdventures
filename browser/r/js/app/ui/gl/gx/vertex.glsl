#version 300 es
/** Vertex shader for GX.
 */
//inputs from CPU
in vec4 in_COL0; //vertex color
in vec3 in_POS;  //vertex position
in vec3 in_NRM;  //vertex normal
in vec2 in_TEX0; //texture coord
in uint in_ID;   //ID for picker

//uniforms
uniform mat4  u_matModelView;   //modelview matrix
uniform mat4  u_matProjection;  //projection matrix
uniform mat4  u_matNormal;      //normal matrix
uniform ivec3 u_ambLightColor;  //ambient light color
uniform ivec3 u_dirLightColor;  //directional light color
uniform vec3  u_dirLightVector; //directional light direction vector

//outputs to next stage
out vec4 vtx_Color;      //vertex color
out vec3 vtx_LightColor; //light color output
out vec2 vtx_TexCoord;   //texture coord
flat out uint  vtx_Id;         //ID for picker

void main() {
    //position gets fed through matrices
    gl_Position = u_matProjection * u_matModelView * vec4(in_POS.xyz, 1);
    //gl_PointSize = 4.0;

    //color gets normalized
    vtx_Color = vec4(in_COL0.r/255.0, in_COL0.g/255.0,
        in_COL0.b/255.0, in_COL0.a/255.0);

    //these just pass through
    vtx_TexCoord = in_TEX0;
    vtx_Id = in_ID;

    //normalize the light vectors
    vec3 ambientLight = vec3(
        float(u_ambLightColor.r) / 255.0,
        float(u_ambLightColor.g) / 255.0,
        float(u_ambLightColor.b) / 255.0);
    vec3 directionalLightColor = vec3(
        float(u_dirLightColor.r) / 255.0,
        float(u_dirLightColor.g) / 255.0,
        float(u_dirLightColor.b) / 255.0);
    vec3 directionalVector = normalize(u_dirLightVector);

    //compute this vertex's light color
    vec4 transformedNormal = u_matNormal * vec4(in_NRM, 1.0);
    float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
    vtx_LightColor = ambientLight + (directionalLightColor * directional);
}
