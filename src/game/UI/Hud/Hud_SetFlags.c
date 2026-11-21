#include <nitro/types.h>

typedef struct UnkStruct_02337688 {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_02337688;

extern UnkStruct_02337688 *gHudTask;

void Hud_SetFlags(s32 v) {
    if (gHudTask != NULL)
        *gHudTask->unk10 = v;
}
