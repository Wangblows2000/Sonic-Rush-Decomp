#include <nitro/types.h>
#pragma opt_common_subs off

typedef struct {
    u32 unk0;
    u8 pad4[0x3A - 4];
    s8 unk3A;
    u8 pad3B[0x62 - 0x3B];
    u16 unk62;
} UnkStruct_0231b834_E;


typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_0231b834_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x68 - 0x14];
    void *unk68;
    u8 pad6C[0x20C - 0x6C];
    s8 unk20C[2];
    u16 unk20E;
} UnkStruct_0231b834;

extern UnkStruct_0231b834_E gCamera[];
extern UnkStruct_0231b834_P *gPlayers[];

void WaterLevelTrigger_State_Idle(void);

void WaterLevelTrigger_State_ApplyOnce(UnkStruct_0231b834 *w) {
    u8 i;
    s32 idx;
    UnkStruct_0231b834_E *e;
    s32 px;

    if (*(u32 *)((u8 *)gCamera + 0xc8) & 1) {
        w->unk20C[0] = gCamera[0].unk3A;
        w->unk20C[1] = gCamera[0].unk3A;
    } else {
        w->unk20C[0] = gCamera[0].unk3A;
        w->unk20C[1] = gCamera[1].unk3A;
    }
    for (i = 0; i < 2; i++) {
        idx = w->unk20C[i];
        if (idx >= 0) {
            e = &gCamera[i];
            px = gPlayers[idx]->unk10;
            if (w->unk10 >= px - 0x4000 && w->unk10 <= px + 0x4000) {
                e->unk0 |= 0x1000000;
                e->unk62 = w->unk20E;
            }
        }
    }
    w->unk68 = WaterLevelTrigger_State_Idle;
}
