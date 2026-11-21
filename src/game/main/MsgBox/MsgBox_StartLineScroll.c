#include <nitro/types.h>

typedef struct {
    u32 unk0;
    void *unk4;
    void *unk8;
    s32 unkc;
    s32 unk10;
} UnkEntry_0204efec;

typedef struct {
    u8 pad0[0x34];
    u32 idx;
    u8 pad38[0x1c];
    u8 unk54[0x180];
    u8 unk1d4[0x180];
    u32 unk354;
} UnkObj_0204efec;

extern UnkEntry_0204efec data_02087cdc[];
extern u32 gSystemFlags;

#pragma thumb on
void MsgBox_StartLineScroll(UnkObj_0204efec *o) {
    u32 idx = o->idx;
    UnkEntry_0204efec *e = &data_02087cdc[idx];
    e->unk0 = o->unk354;
    e->unk4 = o->unk54;
    e->unk8 = o->unk1d4;
    e->unkc = 2;
    e->unk10 = 0;
    gSystemFlags |= 0x400 << idx;
}
#pragma thumb reset
