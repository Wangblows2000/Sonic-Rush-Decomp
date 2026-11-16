#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_02327104_88;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 pad34[0x54 - 0x34];
    u32 unk54;
    u8 pad58[4];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02327104_88 *unk88;
} UnkStruct_02327104;

extern s16 data_0206e470[];

static inline s16 UnkCos_02327104(u16 a) {
    s32 i = (s32)(a & 0xffff) >> 4;
    return data_0206e470[(u32)((i << 1) + 1)];
}

static inline s16 UnkSin_02327104(u16 a) {
    s32 i = (s32)(a & 0xffff) >> 4;
    return data_0206e470[(u32)(i << 1)];
}

void func_ov000_02327104(UnkStruct_02327104 *w) {
    if (w->unk88 != NULL) {
        w->unk10 = w->unk88->unk10 + (w->unk28 << 8);
        w->unk14 = w->unk88->unk14 + (w->unk2A << 8);
        w->unk18 = w->unk88->unk18 + (w->unk2C << 8);
        if (w->unk5C & 1) {
            w->unk10 -= w->unk1C;
            w->unk18 -= w->unk24;
        } else {
            w->unk10 += w->unk1C;
            w->unk18 += w->unk24;
        }
        w->unk14 += w->unk20;
        w->unkC -= 0x600;
        w->unk2E = (w->unk8 * UnkCos_02327104(w->unkC)) >> 12;
        w->unk30 -= 0x20;
        w->unk32 = (w->unk8 * UnkSin_02327104(w->unkC)) >> 12;
        w->unk8 += 0x18;
        w->unk1C += w->unk2E;
        w->unk20 += w->unk30;
        w->unk24 += w->unk32;
    }
    if (w->unk5C & 8) {
        w->unk54 |= 4;
    }
}
