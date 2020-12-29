/** Vertex shader for GX.
 */
attribute vec4 vtxColor;      //vertex color
attribute vec3 vtxPos;        //vertex position
attribute vec3 vtxNormal;     //vertex normal
attribute vec2 vtxTexCoord;   //texture coord
attribute vec4 vtxId;         //ID for picker

uniform mat4  matModelView;   //modelview matrix
uniform mat4  matProjection;  //projection matrix
uniform mat4  matNormal;      //normal matrix
uniform ivec3 ambLightColor;  //ambient light color
uniform ivec3 dirLightColor;  //directional light color
uniform vec3  dirLightVector; //directional light direction vector

varying lowp  vec4 vColor;    //vertex color
varying highp vec3 vLighting; //light color output
varying highp vec2 vTexCoord; //texture coord
varying highp vec4 vId;       //ID for picker

void main() {
    //position gets fed through matrices
    gl_Position = matProjection * matModelView * vec4(vtxPos.xyz, 1);
    //gl_PointSize = 4.0;

    //color gets normalized
    vColor = vec4(vtxColor.r/255.0, vtxColor.g/255.0,
        vtxColor.b/255.0, vtxColor.a/255.0);

    //these just pass through
    vTexCoord = vtxTexCoord;
    vId = vtxId;

    //normalize the light vectors
    highp vec3 ambientLight = vec3(
        float(ambLightColor.r) / 255.0,
        float(ambLightColor.g) / 255.0,
        float(ambLightColor.b) / 255.0);
    highp vec3 directionalLightColor = vec3(
        float(dirLightColor.r) / 255.0,
        float(dirLightColor.g) / 255.0,
        float(dirLightColor.b) / 255.0);
    highp vec3 directionalVector = normalize(dirLightVector);

    //compute this vertex's light color
    highp vec4 transformedNormal = matNormal * vec4(vtxNormal, 1.0);
    highp float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
    vLighting = ambientLight + (directionalLightColor * directional);
}
