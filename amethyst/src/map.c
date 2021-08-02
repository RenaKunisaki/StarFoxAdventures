#include "main.h"

void* mapLoadDataFileHook(MapDirIdx32 map, DataFileEnum file) {
    //replaces the first call to mapLoadDataFile() in mapLoadDataFiles().
    OSReport("LOAD MAP DIR %02X %s", map, mapDirNames[map]);
    return saveMapLoadHook(map, file);
    //return mapLoadDataFile(map, file);
}

void initMapHacks() {
    hookBranch(0x80042ec4, mapLoadDataFileHook, 1);
}
