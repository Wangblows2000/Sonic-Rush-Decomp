#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u16 unk38;
    s8 unk3A;
    u8 pad3B[0x64 - 0x3B];
} UnkStruct_0236aad8_E;

typedef struct {
    UnkStruct_0236aad8_E e[2];
    u32 unkC8;
} UnkStruct_0236aad8;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x212 - 0x20C];
    u16 unk212;
    s8 unk214[2];
} UnkStruct_02309be0;

extern UnkStruct_0236aad8 gCamera;
extern UnkStruct_StageObj *gPlayers[];

void MapObj70_State_Idle(void);

void func_ov000_02309be0(UnkStruct_02309be0 *w) {
    u8 i;
    s32 idx;
    UnkStruct_StageObj *p;
    UnkStruct_0236aad8_E *e;

    if (gCamera.unkC8 & 1) {
        w->unk214[0] = gCamera.e[0].unk3A;
        w->unk214[1] = gCamera.e[0].unk3A;
    } else {
        w->unk214[0] = gCamera.e[0].unk3A;
        w->unk214[1] = gCamera.e[1].unk3A;
    }
    for (i = 0; i < 2; i++) {
        idx = w->unk214[i];
        if (idx >= 0) {
            p = gPlayers[idx];
            e = &gCamera.e[i];
            if (w->base.unk10 >= p->unk10 - 0x4000 && w->base.unk10 <= p->unk10 + 0x4000) {
                e->unk34 |= 0x10;
                e->unk38 = w->unk212;
            }
        }
    }
    w->base.unk68 = MapObj70_State_Idle;
}
