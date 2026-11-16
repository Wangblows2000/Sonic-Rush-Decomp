#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const u8 *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s16 alpha[32];
    u16 delay[32];
} UnkSub_020471ec;

typedef struct {
    u8 pad0[0x344];
    u32 flags;
} UnkObj_020471ec;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern u8 data_020732e8[];
extern u8 data_020732b8[];
void ClearDemo_Unk_02047164(void *);

#pragma thumb on
void ClearDemo_Unk_020471ec(UnkObj_020471ec *o, UnkSub_020471ec *s) {
    u32 i;
    o->flags &= ~0x200;
    o->flags |= 1;
    if (gGameState.unk0 == 0) {
        s->unkc = 0x1b000;
        s->unk10 = 0x1e000;
        s->table = data_020732e8;
        s->count = 0x12;
    } else {
        s->unkc = 0x32000;
        s->unk10 = 0x1e000;
        s->table = data_020732b8;
        s->count = 0xd;
    }
    for (i = 0; i < 32; i++) {
        s->alpha[i] = 0;
        s->delay[i] = i * 4;
    }
    s->func = ClearDemo_Unk_02047164;
}
#pragma thumb reset
