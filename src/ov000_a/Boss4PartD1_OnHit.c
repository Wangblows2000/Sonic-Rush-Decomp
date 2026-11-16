#include <nitro/types.h>

typedef struct UnkStruct_022dc3f0_p {
    u8 unk0;
    u8 pad[0x328 - 1];
    s16 unk328;
    u8 pad1[0x3b2 - 0x32a];
    s16 unk3B2;
} UnkStruct_022dc3f0_p;

typedef struct UnkStruct_022dc3f0_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x13e - 0x58];
    u16 unk13E;
    u8 pad2[0x16a - 0x140];
    u16 unk16A;
    u8 pad3[0x196 - 0x16c];
    u16 unk196;
    u8 pad4[0x254 - 0x198];
    s32 unk254;
    s32 unk258;
    s32 unk25C;
} UnkStruct_022dc3f0_o;

typedef struct UnkStruct_022dc3f0_h {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_022dc3f0_h;

typedef struct UnkStruct_022dc3f0_w {
    u8 pad[0xbdc];
    s32 unkBDC;
} UnkStruct_022dc3f0_w;

extern UnkStruct_022dc3f0_w *data_ov000_02369b2c;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void BossEffect_CreateShootBomb(s32 a, s32 b, s32 c, s32 d);
void func_ov000_0234a9d4(UnkStruct_022dc3f0_p *p, s32 a);
void func_ov000_022de344(void);
void Player_OnHurt(UnkStruct_022dc3f0_h *a, UnkStruct_022dc3f0_h *b);

void Boss4PartD1_OnHit(UnkStruct_022dc3f0_h *a, UnkStruct_022dc3f0_h *b) {
    UnkStruct_022dc3f0_w *w = data_ov000_02369b2c;
    UnkStruct_022dc3f0_o *o = a->unk18;
    UnkStruct_022dc3f0_p *p = b->unk18;
    if (p->unk0 != 1)
        return;
    if (p->unk328 == 0x30) {
        w->unkBDC--;
        if (w->unkBDC < 0)
            w->unkBDC = 0;
        Sound_PlaySeqArc(0, 0, 0x33);
        BossEffect_CreateShootBomb(1, o->unk254 >> 4, -o->unk258 >> 4, o->unk25C >> 4);
        func_ov000_0234a9d4(p, -0x900);
        func_ov000_022de344();
        o->unk54 |= 8;
        o->unk13E |= 0x1000;
        o->unk16A |= 0x1000;
        o->unk196 |= 0x1000;
    } else if (p->unk3B2 == 0) {
        Player_OnHurt(b, a);
    }
}
