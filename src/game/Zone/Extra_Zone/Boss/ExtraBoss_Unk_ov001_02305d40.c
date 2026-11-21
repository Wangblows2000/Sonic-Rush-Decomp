#include <nitro/types.h>

typedef struct UnkStruct_02305d40_t {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_02305d40_t;

typedef struct UnkStruct_02305d40_b {
    u16 a[0x22];
    u16 b[0x20];
    u8 pad84[0x180 - 0x84];
} UnkStruct_02305d40_b;

typedef struct UnkStruct_02305d40 {
    u8 pad0[0x10];
    UnkStruct_02305d40_t *unk10;
    u8 pad14[0x34c - 0x14];
    UnkStruct_02305d40_b bufs[1];
    u8 pad4CC[0x5d8 - 0x4cc];
    s32 unk5D8;
    s16 unk5DC;
    u16 unk5DE;
} UnkStruct_02305d40;

extern UnkStruct_02305d40 *data_ov001_0231f5a8;
extern u32 gSystemFlags;
extern s16 gDisplaySub[];

void ExtraBoss_Unk_ov001_02305cd8(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBoss_Unk_ov001_02305d40(void) {
    s32 v;
    s32 i;
    s32 x;
    u16 c;
    u16 *d;

    data_ov001_0231f5a8->unk5DC += 0x111;
    v = data_ov001_0231f5a8->unk5DC;
    if (v < 0x1000) {
        i = 3;
        x = 0;
        do {
            x = FX_Mul(x - 0x80, v) + 0x80;
        } while (i-- != 0);
        c = x;
        d = data_ov001_0231f5a8->bufs[data_ov001_0231f5a8->unk5DE ^ 1].a;
        for (i = 0; i < 0x20; i++, d++)
            *d = c + 0x80;
        d = data_ov001_0231f5a8->bufs[data_ov001_0231f5a8->unk5DE ^ 1].b;
        for (i = 0; i < 0x20; i++, d++)
            *d = 0x80 - c;
        data_ov001_0231f5a8->unk5DE ^= 1;
        return;
    }
    data_ov001_0231f5a8->unk10->unk8 = ExtraBoss_Unk_ov001_02305cd8;
    data_ov001_0231f5a8->unk5D8 = 10;
    gSystemFlags &= ~0x400;
    gDisplaySub[2] = 0x80;
}
