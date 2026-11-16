#include <nitro/types.h>

typedef struct UnkStruct_022f034c_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    u8 pad118[0x12c - 0x118];
    s16 unk12C;
} UnkStruct_022f034c_o;

typedef struct UnkStruct_022f034c_l {
    u8 pad0[0x18];
    struct UnkStruct_022f034c *unk18;
} UnkStruct_022f034c_l;

typedef struct UnkStruct_022f034c {
    u8 pad0[0x1c];
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f034c_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
    u8 pad1CC[0x1dc - 0x1cc];
    UnkStruct_022f034c_l *unk1DC;
} UnkStruct_022f034c;

extern u16 data_ov001_02316840[];
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;

void ExtraBossObj_State_Unk_ov001_022f0234(void);
void Model_SetAnimation(UnkStruct_022f034c_o *o, u32 slot, void *res, u32 idx, void *tex);
void ExtraBoss_Unk_ov001_022fa830(UnkStruct_022f034c *p, s32 b, s32 c);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void Sound_PlayVoice(u32 *p, u32 a, u32 b);
UnkStruct_022f034c *ExtraBoss_GetPlayer(void);

void ExtraBossObj_State_Unk_ov001_022f034c(UnkStruct_022f034c *p) {
    UnkStruct_022f034c_o *b;
    UnkStruct_022f034c_o *a;
    s32 d;
    UnkStruct_022f034c *t;

    p->unk1C8--;
    if (p->unk1C8 <= 0) {
        a = p->unk84;
        b = p->unk1DC->unk18->unk84;
        p->unk50 = p->unk54 = 0;
        p->unk74 = ExtraBossObj_State_Unk_ov001_022f0234;
        a->unk10C |= 0x40;
        Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0x54], 0);
        Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[0x55], 0);
        a->unk12C = 0x400;
        a->unk10C &= ~4;
        a->unk110 &= ~4;
        a->unk114 = 0x1000;
        b->unk10C |= 0x40;
        Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840[0x56], 0);
        Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840[0x57], 0);
        b->unk12C = 0x400;
        b->unk10C &= ~4;
        b->unk110 &= ~4;
        b->unk114 = 0x1000;
        ExtraBoss_Unk_ov001_022fa830(p, p->unk1C, -0xa000);
        Sound_PlaySeqArc(0, 0, 0xb7);
        Sound_PlayVoice(0, 0, 0xd2);
        return;
    }
    t = ExtraBoss_GetPlayer();
    d = t->unk1C - p->unk1C;
    if (d > 0) {
        if (d > 0x1000)
            d = 0x1000;
        if (p->unk1C + d > 0x111000)
            d = 0x111000 - p->unk1C;
    } else if (d < 0) {
        if (d < -0x1000)
            d = -0x1000;
        if (p->unk1C + d < 0x65000)
            d = 0x65000 - p->unk1C;
    }
    p->unk54 = d;
}
