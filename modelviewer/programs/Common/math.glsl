#define PI 3.141592654


vec2 rotate2d(vec2 vector, float angle) {
    /** Rotate 2D vector.
     *  vector: Input vector.
     *  angle:  Rotation angle in radians.
     *  Returns `vector` rotated by `angle`.
     */
    return mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    ) * vector;
}

float angleBetween2D(vec2 a, vec2 b) {
    /** Compute angle between two vectors.
     *  The vectors must be normalized first, unless you only care about
     *  the sign of the result (which is >0 if <90deg apart,
     *  <0 if >90deg apart, 0 if exactly 90deg apart).
     */
    return acos(dot(a, b));
}

vec3 orthoVector(vec3 v) {
    /** Given a vector v, return an arbitrary vector
     *  which is orthogonal to v.
     */
    return vec3(-v.y, v.x, 0);
}

vec3 faceNormal(vec3 a, vec3 b, vec3 c) {
    /** Given three vertices, calculate their normal vector.
     */
    vec3 A = c - a;
    vec3 B = b - a;
    return normalize(cross(A,B));
}

mat4 rotateX(float angle) {
    /** Generate 3D X rotation matrix.
     */
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
        1, 0,  0, 0,
        0, c, -s, 0,
        0, s,  c, 0,
        0, 0,  0, 1);
}

mat4 rotateY(float angle) {
    /** Generate 3D X rotation matrix.
     */
    float c = cos(angle);
    float s = sin(angle);
	return mat4(
         c,	0, s, 0,
         0,	1, 0, 0,
        -s,	0, c, 0,
         0, 0, 0, 1);
}

mat4 rotateZ(float angle) {
    /** Generate 3D X rotation matrix.
     */
    float c = cos(angle);
    float s = sin(angle);
	return mat4(
        c, -s, 0, 0,
        s,  c, 0, 0,
        0,  0, 1, 0,
        0,  0, 0, 1);
}
