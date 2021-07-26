#include "main.h"

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
    Model *model = objGetCurModelPtr(obj); //replaced
    Color4b color;

    HitState *hits = obj->hitstate;
    for(int iSphere=0; iSphere < model->header->nHitSpheres; iSphere++) {
        HitSpherePos *pos = &model->curHitSpherePos[iSphere];
        HitSphere *sphere = &model->header->hitSpheres[iSphere];
        color.value = 0x7F;
        if(hits) {
            if(!hits->disable) color.value = 0x7F7F7F7F;
            for(int iHit=0; iHit < hits->nHits; iHit++) {
                if(hits->sphereIdxs[iHit] == iSphere) {
                    //XXX not very useful.
                    //maybe we can just map the hit types (there aren't many) over a rainbow.
                    //and/or have one color for "damage" and one for "no damage".
                    //or color by the HitState's own type.
                    color.r = MIN(hits->recordedDamage [iHit] * 8, 255);
                    color.g = MIN(hits->recordedHitType[iHit] * 8, 255);
                    color.b = MIN(hits->sphereIdxs     [iHit] * 8, 255);
                    color.a = 0x7F;
                    break;
                }
            }
        }
        gxSetTexEnvColor(0, &color);
        drawSphere(pos->pos, pos->radius, color);
    }
}

void _renderObjHitboxes(ObjInstance *obj) {
    drawHitbox(obj);

    Model *model = obj->models[obj->curModel];
    for(int iChild=0; iChild < obj->nChildren; iChild++) {
        ObjInstance *child = obj->child[iChild];
        _renderObjHitboxes(child);
    }
}

void renderObjsHook(bool *visible) {
    renderObjects(visible);
    if(debugRenderFlags & DEBUGRENDER_HITBOXES) {
        beginDrawHitboxes();
        for(int iObj=0; iObj < numLoadedObjs; iObj++) {
            _renderObjHitboxes(loadedObjects[iObj]);
        }
        finishDrawingHitboxes();
    }
}

void hitboxHooksInit() {
    hookBranch(0x8005c458, renderObjsHook, 1);
}
