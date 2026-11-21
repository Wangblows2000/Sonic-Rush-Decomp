#include <nitro/types.h>

typedef struct UnkStruct_0230109c {
    u8 pad0[0x44];
    void *objs[0x1e];
} UnkStruct_0230109c;

extern UnkStruct_0230109c *gExtraBossPlayerManager;

void ExtraBossObj_Destroy(void *o);

void ExtraBossObj_DestroyKind1(void) {
    s32 i;

    for (i = 0; i < 0x1e; i++) {
        if (gExtraBossPlayerManager->objs[i] != NULL)
            ExtraBossObj_Destroy(gExtraBossPlayerManager->objs[i]);
    }
}
