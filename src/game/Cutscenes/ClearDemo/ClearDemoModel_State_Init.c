#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u8 pad4[0x114];
    s32 unk118;
    u8 pad11c[0x20];
    s32 unk13c;
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s32 unk14c;
    s32 unk150;
    s16 unk154;
    s16 unk156;
    s16 unk158;
} UnkSub_02046d20;

typedef struct {
    u8 pad0[0x344];
    u32 flags;
} UnkObj_02046d20;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern s32 data_02073478[];
extern s32 data_0207347c[];
extern s32 data_02073480[];
extern s32 data_0207346c[];
extern s32 data_02073470[];
extern s32 data_02073474[];
extern s32 data_02073484[];
extern s16 data_020734b8[];
extern s16 data_020734ba[];
void ClearDemoModel_State_Delay(void *);

#pragma thumb on
void ClearDemoModel_State_Init(UnkObj_02046d20 *o, UnkSub_02046d20 *s) {
    s32 idx = gGameState.unk0;
    s16 v;
    if (idx > 1)
        idx = 0;
    o->flags &= ~0x80;
    o->flags |= 2;
    s->unk13c = data_02073478[idx * 20];
    s->unk140 = data_0207347c[idx * 20];
    s->unk144 = data_02073480[idx * 20];
    s->unk148 = data_0207346c[idx * 20];
    s->unk14c = data_02073470[idx * 20];
    s->unk150 = data_02073474[idx * 20];
    s->unk154 = data_02073484[idx * 20];
    s->unk156 = 0;
    if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2)
        v = data_020734ba[idx * 40];
    else
        v = data_020734b8[idx * 40];
    s->unk158 = v;
    s->unk118 = 0;
    s->func = ClearDemoModel_State_Delay;
}
#pragma thumb reset
