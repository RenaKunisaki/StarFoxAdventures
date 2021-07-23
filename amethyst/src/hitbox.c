#include "main.h"
#include "revolution/os.h"

#if 1 //this is unstable

#define MAX_RECORDED_HITS 16
typedef struct {
    ObjInstance *obj, *outObj;
    int sphereIdx, damage, hitType;
    u8 frameCount;
    char name1[12], name2[12];
} RecordedHit;
static RecordedHit *recordedHits = NULL;
static int lastHitIdx = -1;

int compareRecordedHits(const void *itemA, const void *itemB) {
    RecordedHit *hitA = (RecordedHit*)itemA;
    RecordedHit *hitB = (RecordedHit*)itemB;
    if(hitA->frameCount < hitB->frameCount) return -1;
    if(hitA->frameCount > hitB->frameCount) return  1;
    return 0;
}

void printHits() {
    if(!recordedHits) return;

    RecordedHit *hits[MAX_RECORDED_HITS];
    for(int i=0; i<MAX_RECORDED_HITS; i++) {
        hits[i] = &recordedHits[i];
    }
    quicksort(hits, 0, MAX_RECORDED_HITS-1, compareRecordedHits);

    for(int i=0; i<MAX_RECORDED_HITS; i++) {
        RecordedHit *hit = hits[i];
        if( hit->frameCount) hit->frameCount--;
        if(!hit->frameCount) hit->hitType = 0;
        if(hit->hitType) {
            u8 t = hit->frameCount * 4;
            if(t < 32) t = 32;
            char color[6] = {0x81, t, t, t, t, 0};
            debugPrintf("%sHit " DPRINT_FIXED "%02X %11s > %11s S %d D %d\n\x81\xFF\xFF\xFF\xFF"
                DPRINT_NOFIXED, color, hit->hitType, hit->name1, hit->name2,
                hit->sphereIdx, hit->damage);
        }
    }
}

int getHitsHook(ObjInstance *obj, ObjInstance **outObj, int *outSphereIdx, uint *outDamage) {
    int idx = -1;
    RecordedHit hit, *pHit = NULL;
    HitState *state = obj->hitstate;

    //allocate as needed due to memory starvation at startup.
    if(!recordedHits) {
        recordedHits = allocTagged(sizeof(RecordedHit) * MAX_RECORDED_HITS,
            ALLOC_TAG_LISTS_COL, "debug:recordedHits");
        if(recordedHits) memset(recordedHits, 0, sizeof(RecordedHit) * MAX_RECORDED_HITS);
    }
    if(recordedHits) {
        //record the params since we don't have them by the end
        int lowest = 255;
        for(int i=0; i<MAX_RECORDED_HITS; i++) {
            int n = (i + lastHitIdx + 1) % MAX_RECORDED_HITS;
            RecordedHit *thisHit = &recordedHits[n];
            if(thisHit->obj == obj && (
                (outObj == NULL && thisHit->outObj == NULL) || (outObj && thisHit->outObj == *outObj))) {
                idx = n;
                break;
            }
            else if(thisHit->hitType == 0) {
                idx = n;
                break;
            }
            else if(thisHit->frameCount < lowest) {
                idx = n;
                lowest = thisHit->frameCount;
            }
        }
        if(idx < 0) idx = 0;
        lastHitIdx = idx;
        pHit = &recordedHits[idx];
        hit.obj        = obj;
        hit.outObj     = NULL;
        hit.sphereIdx  = -1;
        hit.damage     = -1;
        hit.frameCount = 60;
    }

    //call the original objGetHits().
    //skip the first instruction since we hooked it.
    //that instruction gets the HitState into r8, so we simulate that by passing it
    //as parameter 6 (and an unused parameter 5).
    ObjInstance *r_outObj = NULL;
    int r_sphereIdx = -1;
    int r_damage    = -1;
    int hitType     = ((int (*)(
        ObjInstance*, ObjInstance**, int*, uint*, void*, HitState*))0x80036880)(
        obj, &r_outObj, &r_sphereIdx, (uint*)&r_damage, NULL, state);

    if(recordedHits) {
        hit.outObj    = r_outObj;
        hit.sphereIdx = r_sphereIdx;
        hit.damage    = r_damage;
        hit.hitType   = hitType;
        getObjName(hit.name1, hit.obj);
        getObjName(hit.name2, hit.outObj);
        memcpy(pHit, &hit, sizeof(RecordedHit));
    }
    if(outObj)       *outObj       = r_outObj;
    if(outSphereIdx) *outSphereIdx = r_sphereIdx;
    if(outDamage)    *outDamage    = r_damage;
    return hitType;
}

void _getHitsHook(void);
__asm__(
    "_getHitsHook:                \n"
    "b      getHitsHook           \n"
);

void hitboxHooksInit() {
    //beginning of objGetHits()
    //XXX maybe also need to hook 80036770 objGetDamageType
    hookBranch(0x8003687c, _getHitsHook, 0);
}

#else

void printHits() {
    //do nothing
}

void hitboxHooksInit() {
    //do nothing
}

#endif
