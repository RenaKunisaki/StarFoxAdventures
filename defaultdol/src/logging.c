#include "main.h"

void _assetLoadHook(AssetDef *def) {
    switch(def->type) {
        case ASSET_TYPE_FILE:
        case ASSET_TYPE_FILE_WITH_OFFSET:
            //these are really the same, offset is a bad name.
            //XXX is it an offset in the final version?
            OSReport("LoadAsset file %s to %08X\n",
                dataFileNames[def->file.id], def->file.buf);
            break;

        case ASSET_TYPE_FILE_WITH_ID_SIZE:
            OSReport("LoadAsset file %s to %08X offs %06X len %06X\n",
                dataFileNames[def->file.id],
                def->file.buf, def->file.offset, def->file.len);
            break;

        case ASSET_TYPE_TEXTURE:
            OSReport("LoadAsset texture %04X to %08X\n",
                def->texture.id, def->texture.buf);
            break;

        case ASSET_TYPE_CHARACTER: {
            OSReport("LoadAsset char to %08X type %04X pos %f, %f, %f ID %08X map %02X obj %08X flags %08X\n",
                *def->character.buf, def->character.objDef->objType,
                def->character.objDef->pos.x, def->character.objDef->pos.y,
                def->character.objDef->pos.z, def->character.objDef->id,
                def->character.mapId, def->character.obj, def->character.flags);
            //OSReport doesn't handle more than a certain number of params
            //(probably I don't handle the stack properly in my patch)
            OSReport("  unused %08X %08X %08X %08X %08X\n",
                def->character.unused04, def->character.unused0C,
                def->character.unused10, def->character.unused20,
                def->character.unk);

            char objName[16];
            strcpy(objName, "???");
            int id = def->character.objDef->objType;
            if(id < 0) id = -id;
            else id = objindex_bin[id];
            ObjData *data = objLoadData(id);
            if(data) strncpy(objName, data->name, OBJECT_NAME_LEN);
            OSReport("  -> \"%s\"\n", objName);

            //XXX if ID is invalid, change it.
            break;
        }

        case ASSET_TYPE_DLL:
            OSReport("LoadAsset DLL %04X exportCount %08X to %08X\n",
                def->dll.id, def->dll.exportCount, def->dll.buf);
            break;

        case ASSET_TYPE_MODEL_INSTANCE:
            OSReport("LoadAsset ModInst %04X flags %08X to %08X\n",
                def->modelInstance.id, def->modelInstance.flags,
                *def->modelInstance.buf);
            break;

        case ASSET_TYPE_ANIMATION:
            OSReport("LoadAsset anim %04X, %04X model %08X to %08X, %08X\n",
                def->animation.id, def->animation.id2, def->animation.model,
                *def->animation.buf, def->animation.dest);
            OSReport("  unused %08X %08X %08X %08X\n",
                def->animation.unused10, def->animation.unused14,
                def->animation.unused18, def->animation.unused1C);
            break;
    }
}

void assetLoadHook(void);
__asm__(
    "assetLoadHook:       \n"
    ASM_FUNC_START(0x100)
    "bl     _assetLoadHook\n"
    ASM_FUNC_END(0x100)
    "lis    4, 0x8007     \n"
    "ori    4, 4, 0x7b6c  \n" //jump back to original code
    "lwz    0, 0x10(1)    \n"
    "mflr   0             \n" //replaced
    "mtctr  4             \n"
    "bctr"
);

uint dvdOpenHook(const char *name) {
    //replaces call to DVDConvertPathToEntrynum() in DVDOpen()
    char curDir[256];
    strcpy(curDir, "???" "/"); //split up to avoid trigraph warning
    DVDGetCurrentDir(curDir, sizeof(curDir));
    OSReport("DVDOpen(%s%s)\n", curDir, name);
    return DVDConvertPathToEntrynum(name);
}

void initLoggingHooks() {
    hookBranch(0x80077b68, assetLoadHook, 0);
    hookBranch(0x8001142c, dvdOpenHook, 1);
}
