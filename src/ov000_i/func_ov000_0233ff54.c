#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u32 flags[2];
    u8 padC[0x54 - 0xc];
    u8 buf0[2][0x300];
    u8 buf1[2][0x300];
} UnkStruct_0233ff54;

typedef struct {
    u32 dst;
    void *src0;
    void *src1;
    u32 unkC;
    u32 unk10;
} UnkStruct_0233ff54_Dma;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkStruct_0233ff54_Pl;

extern UnkStruct_0233ff54_Dma data_02087cdc[];
extern u32 gSystemFlags;
extern UnkStruct_0233ff54_Pl data_ov000_0236aad8[];

void func_ov000_0233ff54(UnkStruct_0233ff54 *w) {
    u8 i;
    UnkStruct_0233ff54_Dma *d;

    for (i = 0; i < 2; i++) {
        if ((data_ov000_0236aad8[i].flags & 8) && !(w->flags[i] & 1)) {
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
