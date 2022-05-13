#include "main.h"
#include "revolution/gx/GXEnum.h"

void begin2D(Color4b *col) {
    /** Set up to render 2D graphics.
     */
    gxResetVtxDescr();
    gxSetVtxDescr(GX_VA_PNMTXIDX,GX_DIRECT);
    gxSetVtxDescr(GX_VA_POS,GX_DIRECT);
    gxSetBackfaceCulling(0);
    gxSetProjection((Mtx*)&hudMatrix,TRUE);
    gxSetBlendMode(GX_BM_BLEND,GX_BL_SRCALPHA,GX_BL_INVSRCALPHA,GX_LO_NOOP);
    gxSetTexEnvColor(0,col);

    //copied from game code. not sure what most of this does.
    GXSetTevKAlphaSel(0,0x1c);
    gxSetTevKsel(0,0xc);
    GXSetTevOrder(0,0xff,0xff,4);
    gxResetIndCmd(0);
    GXSetTevColorIn(0,0xf,0xf,0xf,0xe);
    _gxSetTexColorEnv(0,7,7,7,6);
    gxSetTexColorEnv1(0,0,0);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    gxSetColorCtrl(0,0,0,1,0,0,2);
    gxSetColorCtrl(2,0,0,1,0,0,2);
    GXSetNumChans(1);
    GXSetNumIndStages(0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
}

void write2Dvtx(int x, int y) {
    static volatile u8  *fifoU8  = (volatile u8*) GX_FIFO_BASE;
    static volatile s16 *fifoS16 = (volatile s16*)GX_FIFO_BASE;
    *fifoU8  = 0x3C; //PNMTXIDX
    *fifoS16 = x * hudScale;
    *fifoS16 = y * hudScale;
    *fifoS16 = -8;
}
