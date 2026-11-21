#include <nitro/types.h>

typedef struct UnkStruct_02337634_2 {
    u8 pad[0x28c];
    s32 unk28c;
} UnkStruct_02337634_2;

typedef struct UnkStruct_02337634 {
    u8 pad[0x10];
    UnkStruct_02337634_2 *unk10;
} UnkStruct_02337634;

extern UnkStruct_02337634 *gHudTask;

void Hud_SetRingCount(s32 v) {
    if (gHudTask != NULL)
        gHudTask->unk10->unk28c = v;
}
