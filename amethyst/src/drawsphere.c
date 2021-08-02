#include "main.h"
#include "revolution/gx/GXEnum.h"

const float pi = 3.14159;
const float two_pi = 3.14159 * 2;

void drawSolidVtx(vec3f pos, Color4b *color) {
    static volatile float *fifoFloat = (volatile float*)GX_FIFO_BASE;
    static volatile u8 *fifoU8 = (volatile u8*)GX_FIFO_BASE;
    *fifoFloat = pos.x;
    *fifoFloat = pos.y;
    *fifoFloat = pos.z;
    *fifoU8 = color->r;
    *fifoU8 = color->g;
    *fifoU8 = color->b;
    *fifoU8 = color->a;
}

//increase for more precise spheres at the expense of performance
#define STACKS 8
#define SLICES 8
#define NUM_VTXS (STACKS * SLICES)
static vec3f *vtxs = NULL; //cache for base vtxs for sphere

/** Generate the base vertices for a sphere.
 *  @return the vertices, or NULL if not enough memory.
 *  @note result is cached.
 */
vec3f* _generateSphere() {
    //https://community.khronos.org/t/sphere-generation/49590/2
    if(vtxs) return vtxs;
    vtxs = allocTagged(sizeof(vec3f) * NUM_VTXS, ALLOC_TAG_MODELS_COL, "debug:sphereVtxs");
    if(!vtxs) return NULL;
    registerFreeablePtr((void**)&vtxs, "debug:sphereVtxs");

    for (int stack = 0; stack < STACKS; ++stack) {
        for (int slice = 0; slice < SLICES; ++slice) {
            float y = 2.0 * stack / (float)STACKS - 1.0; //faster
            //float y = -cosf(pi * (float)stack / STACKS); //more precise
            float r = sqrtf(1 - (y * y));
            float x = r * sinf(two_pi * slice / (float)SLICES);
            float z = r * cosf(two_pi * slice / (float)SLICES);
            vec3f v = {x, y, z};
            vtxs[(stack * SLICES) + slice] = v;
        }
    }

    return vtxs;
}

/** Draw one vertex of a sphere.
 *  @param pos Centre position.
 *  @param radius Radius of sphere.
 *  @param color Color to draw.
 *  @param vtx Base vertex to draw.
 */
void _drawSphereVtx(vec3f *pos, float radius, Color4b *color, vec3f *vtx) {
    vec3f v = *vtx;
    v.x = (v.x * radius) + pos->x;
    v.y = (v.y * radius) + pos->y;
    v.z = (v.z * radius) + pos->z;
    drawSolidVtx(v, color);
}

/** Draw a sphere.
 *  @param pos Centre position.
 *  @param radius Radius of sphere.
 *  @param color Color to draw.
 *  @note Resolution is defined at compile time.
 */
void drawSphere(vec3f pos, float radius, Color4b color) {
    vec3f *vtxs = _generateSphere();
    if(!vtxs) return;

    gxSetTexEnvColor(0, &color);

    //draw strips around the perimeter.
    for(int iStack = 0; iStack < STACKS - 1; ++iStack) {
        int s1 = iStack, s2 = iStack + 1;
        if(s2 >= STACKS) s2 = 0;

        GXBegin(GX_TRIANGLESTRIP, 2, SLICES * 2);

        //first two vtxs
        vec3f *v1 = &vtxs[(s1 * SLICES)]; //top left
        vec3f *v2 = &vtxs[(s2 * SLICES)]; //bottom left
        _drawSphereVtx(&pos, radius, &color, v1);
        _drawSphereVtx(&pos, radius, &color, v2);

        //then chain the rest on for the strip
        for(int iSlice = 1; iSlice < SLICES; ++iSlice) {
            int c1 = iSlice, c2 = iSlice + 1;
            if(c2 >= SLICES) c2 = 0;
            vec3f *v3 = &vtxs[(s1 * SLICES) + c2]; //top right
            vec3f *v4 = &vtxs[(s2 * SLICES) + c2]; //bottom right
            _drawSphereVtx(&pos, radius, &color, v3);
            _drawSphereVtx(&pos, radius, &color, v4);
        }
    }

    //manually draw top and bottom
    vec3f top = {0, 0, -1};
    GXBegin(GX_TRIANGLEFAN, 2, SLICES+1);
    _drawSphereVtx(&pos, radius, &color, &top);
    for(int i=0; i<SLICES; i++) {
        vec3f *v = &vtxs[i];
        _drawSphereVtx(&pos, radius, &color, v);
    }

    vec3f bottom = {0, 0, 1};
    GXBegin(GX_TRIANGLEFAN, 2, SLICES+1);
    _drawSphereVtx(&pos, radius, &color, &bottom);
    for(int i=0; i<SLICES; i++) {
        vec3f *v = &vtxs[i + ((STACKS - 1) * SLICES)];
        _drawSphereVtx(&pos, radius, &color, v);
    }
}
