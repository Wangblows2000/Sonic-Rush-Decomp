#include <nitro/types.h>

typedef struct UnkStruct_022d2688_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    u8 pad1[0x90 - 0x48];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x12c - 0x118];
    s16 unk12C;
    u8 pad4[0x138 - 0x12e];
} UnkStruct_022d2688_a;

typedef struct UnkStruct_022d2688_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d2688_h;

typedef struct UnkStruct_022d2688_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d2688_q;

typedef struct UnkStruct_022d2688_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d2688_q unk134;
} UnkStruct_022d2688_o;

typedef struct UnkStruct_022d2688 UnkStruct_022d2688;
struct UnkStruct_022d2688 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x5c - 0x14];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d2688 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d2688_a unkA9C;
    UnkStruct_022d2688_a unkBD4;
    UnkStruct_022d2688_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d2688_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d2688_o *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1450 - 0x13a8];
    s32 unk1450;
    u8 pad5d[0x1458 - 0x1454];
    s32 unk1458;
    s32 unk145C;
    s32 unk1460;
    s32 unk1464;
    s32 unk1468;
    s16 unk146C;
    u8 pad7[0x1470 - 0x146e];
    s32 unk1470;
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022d2920(UnkStruct_022d2688 *s, s32 mode);
void Boss2_State_Unk_ov000_022d255c(UnkStruct_022d2688 *s);

void Boss2_State_Unk_ov000_022d2688(UnkStruct_022d2688 *s) {
    s->unk1304 += s->unk146C;
    if (s->unk1304 >= 0x8000) {
        s->func = Boss2_State_Unk_ov000_022d255c;
        s->unk1304 = 0x8000;
        s->unkF84[6].unkA &= ~4;
        s->unkF84[6].unkA |= 0x1000;
        s->unk5C &= ~1;
        Sound_PlaySeqArc(s->unk1450, 0, 0xa0);
    } else if (s->unk1304 >= 0x4000) {
        func_ov000_022d2920(s, 0);
    }
}
