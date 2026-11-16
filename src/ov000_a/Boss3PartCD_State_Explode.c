#include <nitro/types.h>

typedef struct UnkStruct_022d57b8_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d57b8_v;

typedef struct UnkStruct_022d57b8 UnkStruct_022d57b8;
struct UnkStruct_022d57b8 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(UnkStruct_022d57b8 *s);
    u8 pad3[0x13e - 0x6c];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x20c - 0x16c];
    u32 unk20C;
    u16 unk210;
    u8 pad6[0x214 - 0x212];
    UnkStruct_022d57b8_v unk214;
    u8 pad7[0x22c - 0x220];
    u16 unk22C;
    u8 pad8[0x234 - 0x22e];
    u16 unk234;
    u8 pad9[0x2b4 - 0x236];
    u8 unk2B4[0x24];
    UnkStruct_022d57b8_v unk2D8;
};

void BossArena_CirclePosToArc(s32 a, s32 b, s32 x, s32 z, s32 *out);
void BossEffect_CreateShootBomb(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3PartCD_State_Exploding(UnkStruct_022d57b8 *s);

void Boss3PartCD_State_Explode(UnkStruct_022d57b8 *s) {
    s->unk13E &= ~4;
    s->unk16A |= 4;
    s->unk20C |= 0x10;
    s->unk5C |= 0x20;
    BossArena_CirclePosToArc(0xf42f8, 0x5fe435, s->unk2D8.x, s->unk2D8.z, &s->unk10);
    s->unk10 >>= 4;
    s->unk14 = 0x1e000;
    s->unk20C &= ~2;
    s->unk22C = 0;
    BossEffect_CreateShootBomb(1, s->unk2D8.x >> 4, -s->unk2D8.y >> 4, s->unk2D8.z >> 4);
    Sound_PlaySeqArc(0, 0, 0x96);
    s->unk234 = 0x20;
    s->func = Boss3PartCD_State_Exploding;
}
