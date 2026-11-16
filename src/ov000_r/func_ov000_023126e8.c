#pragma opt_dead_assignments off
#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xC];
    u32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    u8 pad18[0x20 - 0x18];
    s32 unk20;
    u8 pad24[0x2A - 0x24];
    s16 unk2A;
    u8 pad2C[0x1E0 - 0x2C];
    UnkStruct_ObjParam *unk1E0;
} UnkStruct_023126e8;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
} UnkStruct_023126e8_Sys;

extern UnkStruct_023126e8_Sys data_022c4560;
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
void func_ov000_023126e8(UnkStruct_023126e8 *w) {
    u16 f = w->unk1E0->unk4;
    u8 spd = f & 7;
    u16 t = w->unkC;
    s32 y = w->unk20;
    s32 amp = (w->unk1E0->unk9 << 15) >> 16;
    if (spd != 0)
        t = (data_022c4560.unk24 * spd + t) & 0x3ff;
    else
        t = ((data_022c4560.unk24 + (t >> 2)) & 0xff) << 2;
    if (f & 8)
        y = (y + (((s16)amp * SIN((u16)((t << 6) + 0x8000))) >> 12)) << 8;
    else
        y = (y + (((s16)amp * SIN((u16)(t << 6))) >> 12)) << 8;
    w->unk2A = y - w->unk14;
    w->unk14 = y;
}
