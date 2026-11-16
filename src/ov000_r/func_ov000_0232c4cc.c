#pragma opt_dead_assignments off
#include <ov000_b/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0232c7b0_P;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0232c7b0_V;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x70 - 0x6C];
    void *unk70;
    u8 pad74[0x8C - 0x74];
    void **unk8C;
    u8 pad90[0x94 - 0x90];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Spr sprA[6];
    UnkStruct_Spr sprB[6];
    UnkStruct_0232c7b0_P pos[32];
    UnkStruct_0232c7b0_V vel[32];
    s16 state[32];
} UnkStruct_0232c7b0;

typedef struct {
    u8 pad0[0xc8];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_Task *gTaskCurrent;
extern UnkStruct_0236aad8 data_ov000_0236aad8;

void StageObj_Draw2DEx(s32 x, s32 y, s32 a, void *w, UnkStruct_Spr *s0, UnkStruct_Spr *s1, s32 b);

void func_ov000_0232c4cc(void) {
    s16 i;
    UnkStruct_0232c7b0 *w;
    s16 k;

    w = gTaskCurrent->work;
    for (i = 0; i < 32; i++) {
        if (w->state[i] != 0) {
            k = (s16)(w->state[i] & 0xf) - 1;
            w->sprA[k].unk2C &= ~0x80000;
            w->sprB[k].unk2C &= ~0x80000;
            if (data_ov000_0236aad8.unkC8 & 0x200)
                w->state[i] ^= 0x80;
            if (w->state[i] & 0x80)
                w->sprA[k].unk2C |= 0x80000;
            else
                w->sprB[k].unk2C |= 0x80000;
            if ((w->pos[i].y >> 8) < 0xc4) {
                if (w->sprA[k].unk2C & 8)
                    w->unk5C |= 0x1000;
                StageObj_Draw2DEx(w->pos[i].x, w->pos[i].y, 0, w, &w->sprA[k], &w->sprB[k], 0);
                w->sprA[k].unk2C |= 8;
                w->sprB[k].unk2C |= 8;
                w->unk5C &= ~0x1000;
            }
        }
    }
    for (i = 0; i < 6; i++) {
        w->sprA[i].unk2C &= ~8;
        w->sprB[i].unk2C &= ~8;
    }
}
