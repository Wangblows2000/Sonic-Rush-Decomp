#include <nitro/types.h>

typedef struct UnkStruct_022d0690_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    u8 pad1[0x90 - 0x48];
    u8 unk90[0x10c - 0x90];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022d0690_a;

typedef struct UnkStruct_022d0690_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d0690_h;

typedef struct UnkStruct_022d0690 UnkStruct_022d0690;
struct UnkStruct_022d0690 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d0690 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d0690_a unkA9C;
    UnkStruct_022d0690_a unkBD4;
    UnkStruct_022d0690_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d0690_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x1458 - 0x1306];
    s32 unk1458;
    u16 unk145C;
    u8 pad6[0x1460 - 0x145e];
    s32 unk1460;
    u8 pad7[0x1474 - 0x1464];
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss2_SetPalettes(u32 flags);

extern s16 data_0206e470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void Boss2_SetTargetAngle(UnkStruct_022d0690 *s, s32 a);
void Boss2_State_Unk_ov000_022d0614(UnkStruct_022d0690 *s);

void Boss2_State_Unk_ov000_022d0690(UnkStruct_022d0690 *s) {
    UnkStruct_022d0690_a *x;
    UnkStruct_022d0690_a *y;
    UnkStruct_022d0690_a *z;
    if (s->unkBD4.unk10C & 2) {
        s->func = Boss2_State_Unk_ov000_022d0614;
        y = &s->unkBD4;
        s->unk20C &= ~4;
        y->unkB = 2;
        x = &s->unkA9C;
        z = &s->unkD0C;
        MTX_RotY33_(y->unk24, data_0206e470[0], data_0206e470[1]);
        if (s->unk145C) {
            Model_SetAnimation(x, 0, s->unkF7C, 0x1b, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x38, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x57, 0);
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 0x15, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x34, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x51, 0);
        }
        x->unk10C &= ~4;
        y->unk10C &= ~4;
        z->unk10C &= ~4;
        x->unk114 = 0x1000;
        y->unk114 = 0x1000;
        z->unk114 = 0x1000;
        Boss2_SetTargetAngle(s, 0);
        s->unkF84[5].unkA &= ~4;
        s->unkF84[5].unkA |= 0x1000;
        s->unkF84[9].unkA &= ~4;
        s->unkF84[9].unkA |= 0x1000;
        NNS_SndPlayerStopSeq(s->unk1458, 0);
    } else {
        s->unk12F4--;
        if (s->unk12F4 == 0)
            s->unkBD4.unk114 = 0x4000;
    }
}
