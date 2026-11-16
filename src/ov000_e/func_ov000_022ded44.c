#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x17C - 1];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
    u8 pad188[0x328 - 0x188];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_Hit;

typedef struct {
    u8 pad[0x20C];
    void *unk20C;
    u8 pad210[0x21C - 0x210];
    u32 unk21C;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0x13E];
    u8 unk13E;
    u8 unk13F;
    u8 unk140;
} UnkStruct_Fx;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *data_ov000_02369b2c;
extern UnkStruct_02369ac0 data_ov000_02369ac0;

void func_ov000_022dd748(UnkStruct_Boss *b);
void func_ov000_022de018(UnkStruct_Boss *b);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void Sound_PlaySeq(s32 a, s32 b);
void BossEffect_CreateHit(s32 a, s32 x, s32 y, s32 z);
void func_ov000_022de224(UnkStruct_Boss *b);
UnkStruct_Fx *BossEffect_CreateClash(s32 a, s32 x, s32 y, s32 z);
void func_ov000_0234aa80(UnkStruct_Player *p);

void func_ov000_022ded44(UnkStruct_Hit *a, UnkStruct_Hit *h) {
    UnkStruct_Player *p = h->unk18;
    UnkStruct_Boss *b = data_ov000_02369b2c;
    UnkStruct_Fx *fx;

    if (p->unk0 != 1)
        return;
    if (p->unk328 != 0x30)
        return;
    if (a->unk18 == b->unk20C) {
        data_ov000_02369ac0.unk8--;
        b->unk21C |= 1;
        if (data_ov000_02369ac0.unk8 == 0) {
            func_ov000_022dd748(b);
            return;
        }
        func_ov000_022de018(b);
        if (data_ov000_02369ac0.unk8 == 2) {
            func_020537b4(0xb);
            func_020533e0(0x12, 0xf0);
            Sound_PlaySeq(0, 0xf);
        } else {
            func_020537b4(0xb);
            func_020533e0(0x10, 0xf0);
        }
        BossEffect_CreateHit(1, p->unk17C >> 4, -p->unk180 >> 4, p->unk184 >> 4);
    } else {
        func_ov000_022de224(b);
        fx = BossEffect_CreateClash(1, p->unk17C >> 4, -p->unk180 >> 4, p->unk184 >> 4);
        fx->unk13E = 0x13;
        fx->unk13F = 3;
        fx->unk140 = 0;
    }
    func_ov000_0234aa80(p);
}
