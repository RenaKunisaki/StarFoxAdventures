#include "main.h"
#include "revolution/os.h"

static u32 prevState[0x35];

void beginDrawHitboxes() {
    //back up prevous state
    gxGetVtxDescrs(prevState);

    //copied from various functions.
    //possibly some of these calls aren't needed.
    //XXX some of these functions have wrong names.
    gxSetZMode_(1, GX_LESS, 0); //enable depth test, don't update depth buffer
    gxResetVtxDescr();
    gxSetVtxDescr(GX_VA_POS,  GX_DIRECT);
    gxSetVtxDescr(GX_VA_CLR0, GX_DIRECT);
    gxSetMtx43((Mtx*)getCameraMtxs(), 0);
    gxSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetTevAlphaIn(7, GX_CA_APREV, GX_CA_APREV, GX_CA_ZERO, GX_CA_APREV);
    gxSetBackfaceCulling(0);
    gxSetKSel(0, 0x1c);
    gxSetTevKsel(0, 0xc);
    gxSetRasTref(0, 0xff, 0xff, 4);
    gxResetIndCmd(0);
    gxSetTexColorEnv0(0, 0xf, 0xf, 0xf, 0xe);
    _gxSetTexColorEnv(0, 7, 7, 7, 6);
    gxSetTexColorEnv1(0, 0, 0);
    _gxSetTexColorEnv0(0, 0, 0, 0, 1, 0);
    _gxSetTexColorEnv1(0, 0, 0, 0, 1, 0);
    gxSetColorCtrl(0, 0, 0, 1, 0, 0, 2);
    gxSetColorCtrl(2, 0, 0, 1, 0, 0, 2);
    gxSetNumColors(1);
    gxTextureFn_8025b6f0(0);
    gxSetNumTextures(0);
    gxTextureFn_8025c2a0(1);
}

void finishDrawingHitboxes() {
    //XXX reset other state
    //gxSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_DSTCLR, GX_LO_NOOP);
    gxSetVtxDescrs(prevState);
    gxSetBackfaceCulling(1);
}

void drawHitbox(ObjInstance *obj) {
    Model *model = objGetCurModelPtr(obj);
    Color4b color;

    HitState *hits = obj->hitstate;
    for(int iSphere=0; iSphere < model->header->nHitSpheres; iSphere++) {
        HitSpherePos *pos = &model->curHitSpherePos[iSphere];
        HitSphere *sphere = &model->header->hitSpheres[iSphere];
        color.value = 0x7F;
        if(hits && !hits->disable) {
            u8 s=128, v=128;
            for(int iHit=0; iHit < hits->nHits; iHit++) {
                if(hits->sphereIdxs[iHit] == iSphere) {
                    //XXX not very useful.
                    //maybe we can just map the hit types (there aren't many) over a rainbow.
                    //and/or have one color for "damage" and one for "no damage".
                    //or color by the HitState's own type.
                    //color.r = MIN(hits->recordedDamage [iHit] * 8, 255);
                    //color.g = MIN(hits->recordedHitType[iHit] * 8, 255);
                    //color.b = MIN(hits->sphereIdxs     [iHit] * 8, 255);
                    //color.r = MIN(hits->type * 8, 255);
                    //color.a = 0x7F;
                    s = 255; v = 255;
                    break;
                }
            }
            color = hsv2rgb(MIN(hits->type * 8, 255), s, v, 128);
        }
        drawSphere(pos->pos, pos->radius, color);
    }
}

void drawAttachPoints(ObjInstance *obj) {
    for(int iAttach=0; iAttach < obj->file->nAttachPoints; iAttach++) {
        AttachPoint *point = &obj->file->pAttachPoints[iAttach];
        Color4b color = {0, 255, 0, 128};
        vec3f pos = point->pos;
        objGetAttachPointWorldPos(obj, iAttach, &pos.x, &pos.y, &pos.z, 0);
        pos.x -= playerMapOffsetX;
        pos.z -= playerMapOffsetZ;
        vec3s rot = point->rot;

        ObjInstance *obj2 = obj;
        while(obj2) {
            rot.x += obj2->pos.rotation.x;
            rot.y += obj2->pos.rotation.y;
            rot.z += obj2->pos.rotation.z;
            obj2 = obj2->parent;
        }
        drawArrow(pos, rot, 2, color);
    }
}

void drawFocusPoints(ObjInstance *obj) {
    for(int i=0; i<obj->file->numFocusPoints; i++) {
        Color4b color = {255, 0, 0, 128};
        vec3f pos = obj->focusPoints[i];
        pos.x -= playerMapOffsetX;
        pos.z -= playerMapOffsetZ;
        pos.x += obj->pos.pos.x;
        pos.y += obj->pos.pos.y;
        pos.z += obj->pos.pos.z;
        drawSphere(pos, 1, color);
    }
}

void drawUnkPoints(ObjInstance *obj) {
    for(int i=0; i<obj->file->numVecs; i++) {
        Color4b color = {0, 0, 255, 128};
        vec3f pos = obj->pVecs[i];
        pos.x -= playerMapOffsetX;
        pos.z -= playerMapOffsetZ;
        pos.x += obj->pos.pos.x;
        pos.y += obj->pos.pos.y;
        pos.z += obj->pos.pos.z;
        drawSphere(pos, 1, color);
    }
}

void _renderObjHitboxes(ObjInstance *obj) {
    //XXX this is still rendering ghosts
    if(obj->flags_0xb0 & (
        ObjInstance_FlagsB0_IsFreed |
        ObjInstance_FlagsB0_Invisible |
        ObjInstance_FlagsB0_DontUpdate)) return;
    if(obj->pos.flags & ObjInstance_Flags06_Invisible) return;
    if(debugRenderFlags & DEBUGRENDER_HITBOXES) drawHitbox(obj);
    if(debugRenderFlags & DEBUGRENDER_ATTACH_POINTS) drawAttachPoints(obj);
    if(debugRenderFlags & DEBUGRENDER_FOCUS_POINTS) drawFocusPoints(obj);
    if(debugRenderFlags & DEBUGRENDER_UNK_POINTS) drawUnkPoints(obj);

    //recurse into children
    Model *model = obj->models[obj->curModel];
    for(int iChild=0; iChild < obj->nChildren; iChild++) {
        ObjInstance *child = obj->child[iChild];
        _renderObjHitboxes(child);
    }
}

void renderObjsHook(u8 param) {
    //renderObjects(visible);
    WRITE8(0x803db658, param); //replaced call just does this
    if(debugRenderFlags & (DEBUGRENDER_HITBOXES | DEBUGRENDER_ATTACH_POINTS |
        DEBUGRENDER_FOCUS_POINTS | DEBUGRENDER_UNK_POINTS)) {
        beginDrawHitboxes();
        for(int iObj=0; iObj < numLoadedObjs; iObj++) {
            _renderObjHitboxes(loadedObjects[iObj]);
        }
        finishDrawingHitboxes();
    }
}

void hitboxHooksInit() {
    //hookBranch(0x8005c458, renderObjsHook, 1);
    hookBranch(0x8005c72c, renderObjsHook, 1);
}
