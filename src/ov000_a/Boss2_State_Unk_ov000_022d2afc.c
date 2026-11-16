#include <nitro/types.h>

typedef struct UnkStruct_022d2afc_a {
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
} UnkStruct_022d2afc_a;

typedef struct UnkStruct_022d2afc_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d2afc_h;

typedef struct UnkStruct_022d2afc_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d2afc_q;

typedef struct UnkStruct_022d2afc_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d2afc_q unk134;
} UnkStruct_022d2afc_o;

typedef struct UnkStruct_022d2afc UnkStruct_022d2afc;
struct UnkStruct_022d2afc {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x5c - 0x14];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d2afc *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d2afc_a unkA9C;
    UnkStruct_022d2afc_a unkBD4;
    UnkStruct_022d2afc_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d2afc_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d2afc_o *unk131C;
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
    s32 unk146C;
    s32 unk1470;
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

void Boss2_State_Unk_ov000_022d29cc(UnkStruct_022d2afc *s);

void Boss2_State_Unk_ov000_022d2afc(UnkStruct_022d2afc *s) {
    UnkStruct_022d2afc_a *x;
    UnkStruct_022d2afc_a *y;
    UnkStruct_022d2afc_a *z;
    s->unk145C -= s->unk1460;
    x = &s->unkA9C;
    if (s->unk1464 < 0) {
        if (s->unk145C > s->unk146C) {
            s->unk145C = s->unk146C;
            if (x->unk10C & 2) {
                y = &s->unkBD4;
                z = &s->unkD0C;
                Model_SetAnimation(x, 0, s->unkF7C, 7, 0);
                Model_SetAnimation(y, 0, s->unkF7C, 0x27, 0);
                Model_SetAnimation(z, 0, s->unkF7C, 0x43, 0);
                x->unk10C &= ~4;
                y->unk10C &= ~4;
                z->unk10C &= ~4;
                x->unk114 = 0x1000;
                y->unk114 = 0x1000;
                z->unk114 = 0x1000;
                s->func = Boss2_State_Unk_ov000_022d29cc;
            }
        }
    } else {
        if (s->unk145C < s->unk146C) {
            s->unk145C = s->unk146C;
            if (x->unk10C & 2) {
                y = &s->unkBD4;
                z = &s->unkD0C;
                Model_SetAnimation(x, 0, s->unkF7C, 0xa, 0);
                Model_SetAnimation(y, 0, s->unkF7C, 0x2a, 0);
                Model_SetAnimation(z, 0, s->unkF7C, 0x46, 0);
                x->unk10C &= ~4;
                y->unk10C &= ~4;
                z->unk10C &= ~4;
                x->unk114 = 0x1000;
                y->unk114 = 0x1000;
                z->unk114 = 0x1000;
                s->func = Boss2_State_Unk_ov000_022d29cc;
            }
        }
    }
    s->unk10 += s->unk145C;
}
