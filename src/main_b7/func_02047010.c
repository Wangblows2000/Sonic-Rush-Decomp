#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const void *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s32 work[20];
    u16 unk64;
} UnkSub_02047010;

typedef struct {
    u8 pad0[0x344];
    u32 flags;
} UnkObj_02047010;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern s32 data_020732d8[];
extern u8 data_020732c8[];
extern u8 data_02073294[];
extern u8 data_020732a0[];
extern u8 data_020732ac[];
void func_02046f8c(void *);

#pragma thumb on
static inline BOOL IsBossStage(void) {
    BOOL r = FALSE;
    if (data_022c4560.unk1c >= 7 && data_022c4560.unk1c < 9 && data_022c4560.unk1d < 2)
        r = TRUE;
    return r;
}

static inline BOOL IsNormalStage(void) {
    BOOL r = FALSE;
    if (data_022c4560.unk1c < 9 && data_022c4560.unk1d < 2)
        r = TRUE;
    return r;
}

void func_02047010(UnkObj_02047010 *o, UnkSub_02047010 *s) {
    u32 i;
    o->flags &= ~0x800;
    o->flags |= 4;
    if (IsBossStage())
        s->unkc = data_020732d8[3];
    else
        s->unkc = data_020732d8[data_022c4560.unk1d];
    s->unk10 = 0xe0000;
    if (!IsNormalStage()) {
        s->table = data_020732c8;
        s->count = 0xe;
    } else if (IsBossStage()) {
        s->table = data_02073294;
        s->count = 0xa;
    } else if (data_022c4560.unk1d == 0) {
        s->table = data_020732a0;
        s->count = 0xb;
    } else {
        s->table = data_020732ac;
        s->count = 0xb;
    }
    for (i = 0; i < 20; i++)
        s->work[i] = 0;
    s->unk64 = 0;
    s->func = func_02046f8c;
}
#pragma thumb reset
