#include "main.h"
#include "revolution/os.h"

/** Draw an arrow.
 *  @param pos Tip position.
 *  @param rot Rotation.
 *  @param scale Arrow scale.
 *  @param color Color to draw.
 */
void drawArrow(vec3f pos, vec3s rot, float scale, Color4b color) {
    gxSetTexEnvColor(0, &color);

    //just draw a pyramid.
    vec3f a = {( 0.0f * scale),  ( 0.0f * scale),  ( 0.0f * scale)};
    vec3f b = {(-0.5f * scale),  (-0.5f * scale),  ( 2.0f * scale)};
    vec3f c = {( 0.5f * scale),  (-0.5f * scale),  ( 2.0f * scale)};
    vec3f d = {( 0.5f * scale),  ( 0.5f * scale),  ( 2.0f * scale)};
    vec3f e = {(-0.5f * scale),  ( 0.5f * scale),  ( 2.0f * scale)};
    Mtx44 m;
    PSMTXIdentiy(&m);
    mtxRotateByVec3s(&m, &rot);
    vec3f_multByMtx2(&m, &a, &a);
    vec3f_multByMtx2(&m, &b, &b);
    vec3f_multByMtx2(&m, &c, &c);
    vec3f_multByMtx2(&m, &d, &d);
    vec3f_multByMtx2(&m, &e, &e);

    a.x += pos.x;  a.y += pos.y;  a.z += pos.z;
    b.x += pos.x;  b.y += pos.y;  b.z += pos.z;
    c.x += pos.x;  c.y += pos.y;  c.z += pos.z;
    d.x += pos.x;  d.y += pos.y;  d.z += pos.z;
    e.x += pos.x;  e.y += pos.y;  e.z += pos.z;

    //OSReport("a=%f %f %f", a.x, a.y, a.z);
    //OSReport("b=%f %f %f", b.x, b.y, b.z);
    //OSReport("c=%f %f %f", c.x, c.y, c.z);
    //OSReport("d=%f %f %f", d.x, d.y, d.z);
    //OSReport("e=%f %f %f", e.x, e.y, e.z);
    //OSReport("p=%f %f %f scale=%f", pos.x, pos.y, pos.z, scale);

    //draw sides
    GXBegin(GX_TRIANGLEFAN, 2, 6);
    drawSolidVtx(a, &color);
    drawSolidVtx(b, &color);
    drawSolidVtx(c, &color);
    drawSolidVtx(d, &color);
    drawSolidVtx(e, &color);
    drawSolidVtx(b, &color);

    //draw bottom
    GXBegin(GX_TRIANGLESTRIP, 2, 4);
    drawSolidVtx(b, &color);
    drawSolidVtx(c, &color);
    drawSolidVtx(d, &color);
    drawSolidVtx(e, &color);
}
