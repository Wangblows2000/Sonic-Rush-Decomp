#include <nitro/types.h>

typedef struct {
    u32 reg;
    void *a;
    void *b;
    s32 mode;
    s32 unk10;
} UnkHdma_022ea2b0;

extern UnkHdma_022ea2b0 data_02087cdc;
extern u32 gSystemFlags;

typedef struct {
    u8 pad0[0xd6c];
    u8 a[0xeec - 0xd6c];
    u8 b[4];
} UnkStruct_022ea2b0;

#pragma thumb on
void func_ov001_022ea2b0(UnkStruct_022ea2b0 *p) {
    UnkHdma_022ea2b0 *h = &data_02087cdc;

    h->reg = 0x4000014;
    h->a = p->a;
    h->b = p->b;
    h->mode = 2;
    h->unk10 = 0;
    gSystemFlags |= 0x400;
}
#pragma thumb reset
