# SFA functions
.set __restore_gpr,0x802860f4
.set __save_gpr,0x802860a8
.set allocTagged,0x80023cc8 #void* (uint size,AllocTag tag,char *name)
.set ClearArena,0x80240400
.set debugPrintf,0x801378a8
.set DVDReadAsyncPrio,0x80248eac
.set free,0x800233e8
.set loadFileByPath,0x80015ab4 #void* (char *path,uint *outSize)
    # Returns pointer to allocated buffer of contents.
    # outSize: if not NULL, receives file size.
.set logSetColor,0x80137520 # r, g, b, a
.set memcpy,0x80003494 # clobbers: r0, r6
.set memset,0x800033D8 # clobbers: r0, r6, r7
.set mm_free,0x80023800 # wrapper for free()
.set modelLoad,0x80029570 # ModelFileHeader* (
    # int modelNum,ModelFlags_loadCharacter flags,uint *outSize)
    # if modelNum is negative, don't use MODELIND.bin
.set OSReport,0x8007d6dc
.set playerGetNearestObject,0x80036e58 # int idx,ObjInstance *obj,float *outDistance, return ObjInstance*
.set sprintf,0x8028f688
.set storeRegs26,0x802860D8
.set strlen,0x802918a4
.set zlbDecompress,0x8004B658 # void *data,uint compLen,void *out
    # returns with r5 = out + rawLen


# SFA globals
.set dataFileSize,0x8035f0a8 # int32[fileIdx]
.set dataFileBuffers,0x8035f3e8 # void*[fileIdx]
.set debugLogBuffer,0x803aa018
.set debugLogEnd,0x803dbc14 # end of debugLogBuffer
.set loadedFileMapIds,0x8035f548 # int16[fileIdx]
.set buttonsJustPressed,0x803398d0
.set pPlayer,0x803428f8
.set playerId,0x803a32c8 # 0=Krystal, 1=Fox

.set distortionFilterVector, 0x803821c8 #vec3f
.set viewportAspect,         0x803db268 #float
.set motionBlurIntensity,    0x803db62c #float, 32=high, 256=unnoticeable
.set colorFilterColor,       0x803db630 #color
.set fovY,                   0x803dc8a4 #float
.set mapCoords,              0x803dcdc8 #int mapX, mapZ, cellX, cellZ
.set bEnableBlurFilter,      0x803dcdf4 #bool  blur filter
.set bBlurFilterUseArea,     0x803dcdf5 #bool  constrain effect by blurFilterArea
.set bEnableDistortionFilter,0x803dcdf6 #bool  distortion filter
.set bEnableMotionBlur,      0x803dcdf7 #bool  motion blur
.set bEnableMonochromeFilter,0x803dcdf8 #bool  monochrome
.set bEnableSpiritVision,    0x803dcdf9 #bool  spirit vision filter
.set bEnableViewFinderHud,   0x803dcdfa #bool  viewfinder
.set bBiggerBlurFilter,      0x803dcdfb #bool  bigger blur filter
.set bEnableColorFilter,     0x803dce3b #bool  actually int
.set distortionFilterAngle1, 0x803dce3c #float distortion filter angle
.set distortionFilterColor,  0x803dce40 #color distortion filter color
.set distortionFilterAngle2, 0x803dce44 #float distortion filter angle 2
.set blurFilterArea,         0x803dce48 #vec3f blur filter area
.set pCurMapInfo,            0x803dce78 #MapInfoEntry*
.set curMapId,               0x803dcec8 #int
.set physicsTimeScale,       0x803dea9c #float, default 60.0

# SFA file IDs
.set AUDIO_TAB,0x00
.set AUDIO_BIN,0x01
.set SFX_TAB,0x02
.set SFX_BIN,0x03
.set AMBIENT_TAB,0x04
.set AMBIENT_BIN,0x05
.set MUSIC_TAB,0x06
.set MUSIC_BIN,0x07
.set MPEG_TAB,0x08
.set MPEG_BIN,0x09
.set MUSICACT_BIN,0x0a
.set CAMACTIO_BIN,0x0b
.set LACTIONS_BIN,0x0c
.set ANIMCURV_BIN,0x0d
.set ANIMCURV_TAB,0x0e
.set OBJSEQ2C_TAB,0x0f
.set FONTS_BIN,0x10
.set CACHEFON_BIN,0x11
.set CACHEFON_BIN_12,0x12
.set GAMETEXT_BIN,0x13
.set GAMETEXT_TAB,0x14
.set GLOBALMA_BIN,0x15
.set TABLES_BIN,0x16
.set TABLES_TAB,0x17
.set SCREENS_BIN,0x18
.set SCREENS_TAB,0x19
.set VOXMAP_TAB,0x1a
.set VOXMAP_BIN,0x1b
.set WARPTAB_BIN,0x1c
.set MAPS_BIN,0x1d
.set MAPS_TAB,0x1e
.set MAPINFO_BIN,0x1f
.set TEX1_BIN,0x20
.set TEX1_TAB,0x21
.set TEXTABLE_BIN,0x22
.set TEX0_BIN,0x23
.set TEX0_TAB,0x24
.set BLOCKS_BIN,0x25
.set BLOCKS_TAB,0x26
.set TRKBLK_TAB,0x27
.set HITS_BIN,0x28
.set HITS_TAB,0x29
.set MODELS_TAB,0x2a
.set MODELS_BIN,0x2b
.set MODELIND_BIN,0x2c
.set MODANIM_TAB,0x2d
.set MODANIM_BIN,0x2e
.set ANIM_TAB,0x2f
.set ANIM_BIN,0x30
.set AMAP_TAB,0x31
.set AMAP_BIN,0x32
.set BITTABLE_BIN,0x33
.set WEAPONDA_BIN,0x34
.set VOXOBJ_TAB,0x35
.set VOXOBJ_BIN,0x36
.set MODLINES_BIN,0x37
.set MODLINES_TAB,0x38
.set SAVEGAME_BIN,0x39
.set SAVEGAME_TAB,0x3a
.set OBJSEQ_BIN,0x3b
.set OBJSEQ_TAB,0x3c
.set OBJECTS_TAB,0x3d
.set OBJECTS_BIN,0x3e
.set OBJINDEX_BIN,0x3f
.set OBJEVENT_BIN,0x40
.set OBJHITS_BIN,0x41
.set DLLS_BIN,0x42
.set DLLS_TAB,0x43
.set DLLSIMPO_BIN,0x44
.set MODELS_TAB2,0x45
.set MODELS_BIN2,0x46
.set BLOCKS_BIN2,0x47
.set BLOCKS_TAB2,0x48
.set ANIM_TAB2,0x49
.set ANIM_BIN2,0x4a
.set TEX1_BIN2,0x4b
.set TEX1_TAB2,0x4c
.set TEX0_BIN2,0x4d
.set TEX0_TAB2,0x4e
.set TEXPRE_BIN,0x4f
.set TEXPRE_TAB,0x50
.set PREANIM_BIN,0x51
.set PREANIM_TAB,0x52
.set VOXMAP_TAB2,0x53
.set VOXMAP_BIN2,0x54
.set ANIMCURV_BIN2,0x55
.set ANIMCURV_TAB2,0x56
.set ENVFXACT_BIN,0x57
