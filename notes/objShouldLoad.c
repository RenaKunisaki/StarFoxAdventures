objShouldUnload:
if(def == NULL) return 0;
if(def->loadFlags & isManualLoad) return 0;
if(mapAct == -1) return 1;
if(mapAct ==  0) return 0;
if(mapAct < 9) {
	if(def->mapActs1 >> (mapAct-1 & 0x3F) & 1) return 1;
}
else {
	if(def->mapActs2 >> (16 - mapAct & 0x3F) & 1) return 1;
}
if(def->loadFlags & isLevelObject) return 0;
if(def->loadFlags & loadForOtherMap) {
	return map[obj->mapId].objGroup[def->bound];
}
if(obj->override && obj->curSeq >= 0) return 0;
if(obj->parent) return 0;
if(obj->heldBy == NULL) {
	if(obj isn't within a 16x16 map grid of player) return 1;
	if(obj is in an empty map cell) return 1;
	if(def->loadFlags & isBlockObject) return 0;
	if(def->loadFlags & OnlyLoadIfNearPlayer && player exists && obj isn't being held) {
		pos = player cur pos
	}
	else {
		pos = player spawn pos?
	}
	if((obj->loadDistance + 40) ** 2 <= dist(pos, obj->pos)) return 1;
	else return 0;
}
