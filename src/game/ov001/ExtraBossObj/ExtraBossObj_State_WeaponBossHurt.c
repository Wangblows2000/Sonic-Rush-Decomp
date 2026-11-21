#include <nitro/types.h>

typedef struct UnkStruct_022f4a50_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    u8 pad118[0x12c - 0x118];
    s16 unk12C;
} UnkStruct_022f4a50_o;

typedef struct UnkStruct_022f4a50_l {
    u8 pad0[0x18];
    struct UnkStruct_022f4a50 *unk18;
} UnkStruct_022f4a50_l;

typedef struct UnkStruct_022f4a50 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f4a50_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s16 unk1C8;
    u8 pad1CA[2];
    s16 unk1CC;
    u8 pad1CE[2];
    s32 unk1D0;
    u8 pad1D4[0x1dc - 0x1d4];
    UnkStruct_022f4a50_l *unk1DC;
} UnkStruct_022f4a50;

extern u16 gExtraBossHandBossAnimTable[];
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern s32 data_ov001_02316820[];
extern s32 data_ov001_0231f070;
extern void *gExtraBossArchive;
extern s32 data_ov001_0231d518[];
extern u16 data_ov001_02316930[];
extern u16 data_ov001_02316918[];
extern u16 data_ov001_02316924[];

void ExtraBossObj_State_WeaponBossHurtRecover(void);
void Model_SetAnimation(UnkStruct_022f4a50_o *o, u32 slot, void *res, u32 idx, void *tex);
s32 FX_DivS32(s32 n, s32 d);
void ExtraBoss_LoadModelPalettes(s32 a, void *b, s32 *c, u16 *d, u16 *e, s32 n);


static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBossObj_State_WeaponBossHurt(UnkStruct_022f4a50 *p) {
    UnkStruct_022f4a50_o *o = p->unk84;
    s32 x;
    s32 i;

    p->unk1CC += 0x43;
    if (p->unk1CC > 0x1000)
        p->unk1CC = 0x1000;
    if (o->unk10C & 2) {
        p->unk74 = ExtraBossObj_State_WeaponBossHurtRecover;
        Model_SetAnimation(o, 0, data_ov001_0231f068, 0, 0);
        o->unk10C |= 4;
        p->unk50 = FX_DivS32(0x100000 - p->unk18, 0xb4);
        p->unk54 = FX_DivS32(0xc0000 - p->unk1C, 0xb4);
        p->unk1D0 = 0xb4;
    } else {
        i = 1;
        x = 0;
        do {
            x = FX_Mul(x - 0x6000, p->unk1CC) + 0x6000;
        } while (i-- != 0);
        p->unk50 = x;
    }
    p->unk1C8++;
    if ((p->unk1C8 & 7) == 1) {
        ExtraBoss_LoadModelPalettes(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d518, data_ov001_02316930, data_ov001_02316924, 5);
    } else if ((p->unk1C8 & 7) == 5) {
        ExtraBoss_LoadModelPalettes(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d518, data_ov001_02316918, data_ov001_02316924, 5);
    }
}
