#include <nitro/types.h>

typedef struct UnkStruct_022d6f20_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d6f20_ctl2;

typedef struct UnkStruct_022d6f20_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d6f20_ctl2 *unk8;
} UnkStruct_022d6f20_ctl;

typedef struct UnkStruct_022d6f20_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d6f20_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6f20_a;

typedef struct UnkStruct_022d6f20 UnkStruct_022d6f20;
struct UnkStruct_022d6f20 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6f20 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6f20_a unk2C4;
    void *unk434;
};

void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d6f20_b {
    u8 pad[0x222];
    u16 unk222;
} UnkStruct_022d6f20_b;

extern UnkStruct_022d6f20_b *data_ov000_02369b14;
extern u32 data_0208fe70;
extern u8 data_ov000_02363080[];
extern u16 data_ov000_023630c4[];

void func_ov000_022d6ed4(UnkStruct_022d6f20 *s);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022d6f20(UnkStruct_022d6f20 *s) {
    func_02031a68(&s->unk2C4, 0, s->unk434, 1, 0);
    s->unk2C4.unk10C |= 4;
    s->unk2C4.unk114 = 0x1000;
    s->unk25A = data_ov000_02363080[data_ov000_02369b14->unk222] + (data_ov000_023630c4[data_ov000_02369b14->unk222] & Rand());
    s->func = func_ov000_022d6ed4;
    s->func(s);
}
