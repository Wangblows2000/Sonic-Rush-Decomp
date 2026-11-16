#include <nitro/types.h>

typedef struct {
    u32 flags[2];
    u8 pad8[0x4C - 8];
    u8 buf0[2][0x300];
    u8 buf1[2][0x300];
} UnkStruct_0233f5a4;

typedef struct {
    u32 dst;
    void *src0;
    void *src1;
    u32 unkC;
    u32 unk10;
} UnkStruct_0233f5a4_Dma;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkStruct_0233f5a4_Pl;

extern UnkStruct_0233f5a4_Dma data_02087cdc[];
extern u32 gSystemFlags;
extern UnkStruct_0233f5a4_Pl gCamera[];

void func_ov000_0233f5a4(UnkStruct_0233f5a4 *w) {
    u8 i;
    UnkStruct_0233f5a4_Dma *d;

    for (i = 0; i < 2; i++) {
        if ((gCamera[i].flags & 8) && !(w->flags[i] & 1)) {
            d = &data_02087cdc[i];
            d->dst = (i == 0) ? 0x4000014 : 0x4001014;
            d->src0 = w->buf0[i];
            d->src1 = w->buf1[i];
            d->unkC = 4;
            d->unk10 = 0;
            gSystemFlags |= 0x400 << i;
            w->flags[i] |= 1;
        }
    }
}
