#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_0232a534_P;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u8 pad58[0x88 - 0x58];
    UnkStruct_0232a534_P *unk88;
} UnkStruct_0232a534;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_0232a534_S;

extern UnkStruct_0232a534_S data_ov000_0236aad8;

void func_ov000_022c5e80(s32 a, s32 b, s32 x, s32 *ox, s32 *oz, u16 *ang);

void func_ov000_0232a534(UnkStruct_0232a534 *w) {
    u16 ang;
    s32 ox;
    s32 oz;

    if (w->unk88 != NULL) {
        w->unk10 = w->unk88->unk10 + w->unk2E;
        w->unk14 = w->unk88->unk14 + w->unk30;
        func_ov000_022c5e80(data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4, w->unk10 << 4, &ox, &oz, &ang);
        w->unk10 = ox >> 4;
        w->unk18 = oz >> 4;
    }
    if (w->unkC > 0x10)
        w->unk30 -= 0xc0;
    w->unkC++;
    if (w->unkC > 0x30)
        w->unk54 |= 4;
}
