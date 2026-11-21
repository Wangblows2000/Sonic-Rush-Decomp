#include <nitro/types.h>

typedef struct UnkStruct_02357090 {
    u8 pad[0x28];
    s32 unk28;
    s32 unk2C;
    u8 pad1[4];
    s32 unk34;
    u8 pad2[4];
    s32 unk3C;
    u8 pad3[0x19c - 0x40];
    s32 unk19C;
} UnkStruct_02357090;

#pragma thumb on
void PlayerSelectMenu_SetModelSlotSelection(UnkStruct_02357090 *s, s32 v) {
    if (s->unk28 == v)
        return;
    s->unk2C = s->unk28;
    s->unk28 = v;
    s->unk34 = 3;
    if (v != 1)
        s->unk3C = -1;
    if (v != 0)
        s->unk19C = -1;
}
#pragma thumb reset
