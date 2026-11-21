#include <nitro/types.h>

typedef struct UnkStruct_02337618_2 {
    u8 pad[0x67c];
    s32 unk67c;
} UnkStruct_02337618_2;

typedef struct UnkStruct_02337618 {
    u8 pad[0x10];
    UnkStruct_02337618_2 *unk10;
} UnkStruct_02337618;

extern UnkStruct_02337618 *gHudTask;

void Hud_SetTime(s32 v) {
    if (gHudTask != NULL)
        gHudTask->unk10->unk67c = v;
}
