#include <nitro/types.h>

typedef struct UnkStruct_0235b08c {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_0235b08c;

extern UnkStruct_0235b08c *gRingManagerTask;

void RingManager_SetFlags(s32 v) {
    if (gRingManagerTask != NULL)
        *gRingManagerTask->unk10 = v;
}
