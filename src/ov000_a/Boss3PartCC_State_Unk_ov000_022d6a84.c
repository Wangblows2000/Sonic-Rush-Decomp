#include <nitro/types.h>

typedef struct UnkStruct_022d6a84_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0x10c - 0x54];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6a84_a;

typedef struct UnkStruct_022d6a84 UnkStruct_022d6a84;
struct UnkStruct_022d6a84 {
    u8 pad[0x14];
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6a84 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6a84_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

void Boss3PartCC_State_Unk_ov000_022d69b4(UnkStruct_022d6a84 *s);

void Boss3PartCC_State_Unk_ov000_022d6a84(UnkStruct_022d6a84 *s) {
    s->unk30 += 0x60;
    if (s->unk14 > 0x20000) {
        s->unk14 = 0x20000;
        s->unk30 = 0;
    }
    if (!(s->unk2C4.unk10C & 2))
        return;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 9, 0);
    s->unk2C4.unk10C |= 4;
    s->unk2C4.unk114 = 0x1000;
    s->func = Boss3PartCC_State_Unk_ov000_022d69b4;
}
