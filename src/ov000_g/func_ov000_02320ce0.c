#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x190 - 0x18];
    void *unk190;
} UnkStruct_02320ce0_T;

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 unk3;
    u8 pad4[4];
    u32 unk8;
    u8 padC[4];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x5C - 0x24];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02320ce0_T *unk88;
    u8 pad8C[0x9C - 0x8C];
    u16 unk9C;
} UnkStruct_02320ce0;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02320ce0_Sys;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_02320ce0_Stage;

extern UnkStruct_02320ce0_Sys data_022c4560;
extern UnkStruct_02320ce0_Stage data_ov000_0236aad8;
extern void *data_ov000_0236ac60;

s32 ObjShift_Set(s32 v, s32 a, s32 b, u16 c, s32 d);
void func_ov000_02320be8(UnkStruct_02320ce0 *w, void *p);

void func_ov000_02320ce0(UnkStruct_02320ce0 *w) {
    UnkStruct_02320ce0_T *t = w->unk88;
    s32 min;

    if (t != NULL) {
    if (!(data_022c4560.unk20 & 0xf)) {
        w->unk2 += 8 - (Rand() & 0xf);
        w->unk3 += 8 - (Rand() & 0xf);
        if ((s8)w->unk2 > 0x20)
            w->unk2 = 0x20;
        if ((s8)w->unk2 < -0x20)
            w->unk2 = -0x20;
        if ((s8)w->unk3 > 0x20)
            w->unk3 = 0x20;
        if ((s8)w->unk3 < -0x20)
            w->unk3 = -0x20;
    }
    w->unk1C = w->unk10;
    w->unk20 = w->unk14;
    w->unk10 = ObjShift_Set(w->unk10, w->unk88->unk10 + ((s8)w->unk2 << 8), 4, w->unk8, 4);
    w->unk14 = ObjShift_Set(w->unk14, w->unk88->unk14 + ((s8)w->unk3 << 8), 4, w->unk8, 4);
    min = (data_ov000_0236aad8.unk62 + 8) << 8;
    if (w->unk14 < min)
        w->unk14 = min;
    if (w->unk9C & 1) {
        if (w->unk5C & 8) {
            StageObj_SetAnimation(w, (u16)(w->unk9C - 1));
            w->unk5C |= 4;
            w->unk5C ^= 1;
        }
    } else if (w->unk5C & 1) {
        if (w->unk1C < w->unk10 - 0x60)
            StageObj_SetAnimation(w, (u16)(w->unk9C + 1));
    } else {
        if (w->unk1C > w->unk10 + 0x60)
            StageObj_SetAnimation(w, (u16)(w->unk9C + 1));
    }
    if (t->unk190 != NULL)
        func_ov000_02320be8(w, t->unk190);
    } else {
        func_ov000_02320be8(w, data_ov000_0236ac60);
    }
}
