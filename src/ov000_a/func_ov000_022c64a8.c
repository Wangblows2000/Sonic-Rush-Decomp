#include <nitro/types.h>

typedef struct UnkStruct_022c64a8 UnkStruct_022c64a8;

typedef struct UnkStruct_022c64a8_2 {
    u8 pad[0x210];
    UnkStruct_022c64a8 *unk210;
    u8 pad1[0x228 - 0x214];
    s16 unk228;
} UnkStruct_022c64a8_2;

struct UnkStruct_022c64a8 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c64a8 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c64a8_2 *gBoss1;

void func_ov000_022cc224(void);
s32 func_0200225c(s32 a, s32 b);
void Boss1Neck_State_Unk_ov000_022c8a84(UnkStruct_022c64a8 *s);
void Boss1Hammer_State_Unk_ov000_022c62a0(UnkStruct_022c64a8 *s);

void func_ov000_022c64a8(UnkStruct_022c64a8 *s) {
    s32 v;
    s->unk2CE = -s->unk2CE;
    func_ov000_022cc224();
    s->unk2C0 = 0;
    v = (s16)s->unk2E;
    if (v < 0)
        v = -v;
    s->unk2C2 = func_0200225c(v << 4, 0x78000);
    s->unk2C2 >>= 1;
    gBoss1->unk210->unk10 = s->unk10;
    gBoss1->unk210->func = Boss1Neck_State_Unk_ov000_022c8a84;
    s->func = Boss1Hammer_State_Unk_ov000_022c62a0;
    s->func(s);
}
