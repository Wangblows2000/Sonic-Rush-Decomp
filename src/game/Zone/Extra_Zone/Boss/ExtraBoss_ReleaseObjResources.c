#include <nitro/types.h>

typedef struct { u32 flags; u8 pad[0x18]; } UnkStruct_0231d6b4;
typedef struct { s32 id; void *anim[5]; } UnkStruct_0231f098;
extern UnkStruct_0231d6b4 gExtraBossFxResTable[];
extern UnkStruct_0231f098 gExtraBossFxRes[];
void NNS_G3dResDefaultRelease(s32 id);

void ExtraBoss_ReleaseObjResources(void) {
    s32 i, j;
    for (i = 0; i < 0x1b; i++) {
        if (gExtraBossFxResTable[i].flags & 1) {
            NNS_G3dResDefaultRelease(gExtraBossFxRes[i].id);
        }
        for (j = 0; j < 6; j++) {
            ((s32 *)&gExtraBossFxRes[i])[j] = 0;
        }
    }
}
