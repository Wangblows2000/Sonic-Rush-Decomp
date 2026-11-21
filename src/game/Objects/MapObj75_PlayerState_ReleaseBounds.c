#include <game/Stage/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x4C - 4];
    s32 unk4C[4];
    s8 unk5C[4];
    u8 pad60[0x64 - 0x60];
} UnkStruct_Plr;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C[10];
    void *unk234[2];
} UnkStruct_02303310;

extern UnkStruct_Plr gCamera[];

void MapObj75_PlayerState_WaitEnter(void);

void MapObj75_PlayerState_ReleaseBounds(UnkStruct_02303310 *w, s32 i) {
    if (w->unk20C[i] & 4) {
        if (--gCamera[i].unk5C[0] == 0) {
            gCamera[i].unk4C[0] = gCamera[2].unk4C[0];
            gCamera[i].unk0 &= ~0x10;
            w->unk20C[i] &= ~4;
        }
    }
    if (w->unk20C[i] & 8) {
        if (--gCamera[i].unk5C[1] == 0) {
            gCamera[i].unk4C[1] = gCamera[2].unk4C[1];
            gCamera[i].unk0 &= ~0x20;
            w->unk20C[i] &= ~8;
        }
    }
    if (w->unk20C[i] & 0x10) {
        if (--gCamera[i].unk5C[2] == 0) {
            gCamera[i].unk4C[2] = gCamera[2].unk4C[2];
            gCamera[i].unk0 &= ~0x10;
            w->unk20C[i] &= ~0x10;
        }
    }
    if (w->unk20C[i] & 0x20) {
        if (--gCamera[i].unk5C[3] == 0) {
            gCamera[i].unk4C[3] = gCamera[2].unk4C[3];
            gCamera[i].unk0 &= ~0x20;
            w->unk20C[i] &= ~0x20;
        }
    }
    w->unk20C[i] &= ~2;
    w->unk234[i] = MapObj75_PlayerState_WaitEnter;
}
