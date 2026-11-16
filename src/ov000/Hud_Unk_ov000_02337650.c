#include <nitro/types.h>

typedef struct UnkStruct_02337650_2 {
    u8 pad[0x14];
    s32 unk14[1];
} UnkStruct_02337650_2;

typedef struct UnkStruct_02337650 {
    u8 pad[0x10];
    UnkStruct_02337650_2 *unk10;
} UnkStruct_02337650;

extern UnkStruct_02337650 *gHudTask;

s32 Hud_Unk_ov000_02337650(s32 i) {
    return gHudTask->unk10->unk14[i];
}
