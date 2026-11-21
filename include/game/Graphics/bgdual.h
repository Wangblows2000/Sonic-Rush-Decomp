#ifndef MAIN_A2_BGDUAL_H
#define MAIN_A2_BGDUAL_H

#include <nitro/types.h>

typedef struct UnkBgRes {
    u32 unk0;
    u32 unk4;
    u32 ofs8;
    u32 ofsC;
    u32 ofs10;
    u32 flags;
    u16 unk18;
    u16 unk1a;
    u16 unk1c;
    u16 unk1e;
} UnkBgRes;

#define UNK_BG_SETUP_COMMON(b, res, a)               \
    do {                                              \
        (b)->unk0 = (a);                              \
        if ((res)->flags & 0x10) {                    \
            (b)->unk0 |= 0x1000;                      \
        }                                             \
        (b)->unk4 = 0;                                \
        (b)->unk8 = 0;                                \
    } while (0)

typedef struct UnkBgDual {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 idx;
    u8 unk10;
    u8 unk11[3];
    u32 unk14;
    u32 unk18;
    u32 unk1c;
    u32 unk20;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2a;
    u16 unk2c;
    u16 unk2e;
    void *unk30;
    void *unk34;
    void *unk38;
    u16 unk3c;
    u16 unk3e;
    struct {
        u32 a;
        u32 b;
    } unk40[2];
    u8 unk50[2];
    u8 unk52[2];
    u32 unk54[2];
    u32 unk5c[2];
    u32 unk64[2];
    u32 unk6c[2];
    u16 unk74[2];
    u16 unk78[2];
    struct {
        u16 a;
        u16 b;
    } unk7c[2];
} UnkBgDual;

#endif
