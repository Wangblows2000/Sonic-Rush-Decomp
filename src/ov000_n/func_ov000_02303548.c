#pragma opt_common_subs off
#pragma opt_loop_invariants off
#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x1E - 4];
    s16 unk1E;
    s16 unk20;
    u8 pad22[0x3A - 0x22];
    s8 unk3A;
    u8 pad3B[0x64 - 0x3B];
} UnkStruct_Plr;

typedef struct {
    u8 pad0[6];
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
} UnkStruct_Prm;

typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} UnkStruct_Rect;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_Rect unk214[2];
    void *unk234[2];
    s8 unk23C[2];
} UnkStruct_02303548;

extern UnkStruct_Plr data_ov000_0236aad8[];

void func_ov000_02303310(void);
void MapObj75_State_Unk_ov000_023034f8(void);

void func_ov000_02303548(UnkStruct_02303548 *s) {
#define P ((UnkStruct_Plr *)(u32)data_ov000_0236aad8)
    UnkStruct_Plr *p;
    u8 i;
    if (P[2].unk0 & 1) {
        s->unk23C[0] = P[0].unk3A;
        s->unk23C[1] = -1;
        s->unk234[0] = func_ov000_02303310;
        s->unk234[1] = NULL;
        s->unk214[0].x0 = s->base.unk10 + ((UnkStruct_Prm *)s->base.unk1E0)->unk6 * P[0].unk1E;
        s->unk214[0].y0 = s->base.unk14 + ((UnkStruct_Prm *)s->base.unk1E0)->unk7 * P[0].unk20;
        s->unk214[0].x1 = s->base.unk10 + P[0].unk1E * (((UnkStruct_Prm *)s->base.unk1E0)->unk6 + ((UnkStruct_Prm *)s->base.unk1E0)->unk8);
        s->unk214[0].y1 = s->base.unk14 + P[0].unk20 * (((UnkStruct_Prm *)s->base.unk1E0)->unk7 + ((UnkStruct_Prm *)s->base.unk1E0)->unk9);
    } else {
        for (i = 0; i < 2; i++) {
            p = &P[i];
            s->unk23C[i] = p->unk3A;
            s->unk234[i] = func_ov000_02303310;
            s->unk234[1] = func_ov000_02303310;
            s->unk214[i].x0 = s->base.unk10 + ((UnkStruct_Prm *)s->base.unk1E0)->unk6 * p->unk1E;
            s->unk214[i].y0 = s->base.unk14 + ((UnkStruct_Prm *)s->base.unk1E0)->unk7 * p->unk20;
            s->unk214[i].x1 = s->base.unk10 + p->unk1E * (((UnkStruct_Prm *)s->base.unk1E0)->unk6 + ((UnkStruct_Prm *)s->base.unk1E0)->unk8);
            {UnkStruct_Prm *q = (UnkStruct_Prm *)s->base.unk1E0;
            s->unk214[i].y1 = s->base.unk14 + p->unk20 * (q->unk7 + q->unk9);}
        }
    }
    s->base.unk68 = MapObj75_State_Unk_ov000_023034f8;
}
