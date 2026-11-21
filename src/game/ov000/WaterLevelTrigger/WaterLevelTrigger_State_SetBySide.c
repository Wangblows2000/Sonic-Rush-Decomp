#include <ov000_b/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x62 - 4];
    u16 unk62;
} UnkStruct_0231b8f8_E;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_0231b8f8_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x1E0 - 0x14];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x20C - 0x1E4];
    s8 unk20C[2];
    u16 unk20E;
} UnkStruct_0231b8f8;

extern UnkStruct_0231b8f8_E gCamera[];
extern UnkStruct_0231b8f8_P *gPlayers[];

void WaterLevelTrigger_State_SetBySide(UnkStruct_0231b8f8 *w) {
    u8 i;
    s32 idx;
    UnkStruct_0231b8f8_E *e;
    BOOL f;

    for (i = 0; i < 2; i++) {
        idx = w->unk20C[i];
        if (idx >= 0) {
            e = &gCamera[i];
            if (gPlayers[idx]->unk10 < w->unk10) {
                if (w->unk1E0->unk4 & 1)
                    f = FALSE;
                else
                    f = TRUE;
            } else {
                if (w->unk1E0->unk4 & 1)
                    f = TRUE;
                else
                    f = FALSE;
            }
            if (f) {
                e->unk0 |= 0x1000000;
                e->unk62 = w->unk20E;
                if (w->unk1E0->unk4 & 8)
                    e->unk0 |= 0x2000000;
            } else {
                e->unk0 &= ~0x1000000;
                e->unk62 = 0xffff;
                e->unk0 &= ~0x2000000;
            }
        }
    }
}
