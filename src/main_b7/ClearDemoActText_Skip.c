#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const u8 *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s32 work[20];
    u16 unk64;
} UnkSub_02046e08;

typedef struct {
    u8 pad0[0x200];
    UnkSub_02046e08 sub;
    u8 pad[0x344 - 0x200 - sizeof(UnkSub_02046e08)];
    u32 flags;
} UnkObj_02046e08;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern s32 data_020732d8[];
extern u16 data_020733dc[];
void ClearDemoActText_State_Init(UnkObj_02046e08 *o, UnkSub_02046e08 *s);
void ClearDemoActText_State_Done(void *);

#pragma thumb on
static inline BOOL IsBossStage(void) {
    BOOL r = FALSE;
    if (gGameState.unk1c >= 7 && gGameState.unk1c < 9 && gGameState.unk1d < 2)
        r = TRUE;
    return r;
}

void ClearDemoActText_Skip(UnkObj_02046e08 *o) {
    UnkSub_02046e08 *s = &o->sub;
    s32 acc = 0;
    s32 i;
    ClearDemoActText_State_Init(o, s);
    o->flags |= 0x3c04;
    if (IsBossStage())
        s->unkc = data_020732d8[3];
    else
        s->unkc = data_020732d8[gGameState.unk1d];
    s->unk10 = 0xb4000;
    for (i = 0; i < s->count; i++) {
        s->work[i] = acc;
        acc += data_020733dc[s->table[i]] << 12;
    }
    s->func = ClearDemoActText_State_Done;
}
#pragma thumb reset
