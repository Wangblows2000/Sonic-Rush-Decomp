#include <nitro/types.h>

typedef struct {
    u32 reg;
    void *a;
    void *b;
    s32 mode;
    s32 unk10;
} UnkHdma_022e833c;

extern UnkHdma_022e833c data_02087cdc;
extern u32 gSystemFlags;

typedef struct {
    u8 pad0[0x70];
    u8 a[0x370 - 0x70];
    u8 b[4];
} UnkStruct_022e833c;

#pragma thumb on
void func_ov001_022e833c(UnkStruct_022e833c *p) {
    UnkHdma_022e833c *h = &data_02087cdc;

    h->reg = 0x4000038;
    h->a = p->a;
    h->b = p->b;
    h->mode = 4;
    h->unk10 = 0;
    gSystemFlags |= 0x400;
}
#pragma thumb reset
