#include <nitro/types.h>

typedef struct UnkStruct_023375f8_2 {
    u8 pad[0x928];
    u16 unk928;
} UnkStruct_023375f8_2;

typedef struct UnkStruct_023375f8 {
    u8 pad[0x10];
    UnkStruct_023375f8_2 *unk10;
} UnkStruct_023375f8;

extern UnkStruct_023375f8 *gHudTask;

void Hud_Unk_ov000_023375f8(u16 v) {
    if (gHudTask != NULL)
        gHudTask->unk10->unk928 = v;
}
