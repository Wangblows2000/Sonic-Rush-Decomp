#include <nitro/types.h>

typedef struct UnkStruct_0235b0a8 {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_0235b0a8;

extern UnkStruct_0235b0a8 *gRingManagerTask;

s32 func_ov000_0235b0a8(void) {
    if (gRingManagerTask == NULL)
        return 0;
    return *gRingManagerTask->unk10;
}
