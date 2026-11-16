#include <nitro/types.h>

typedef struct UnkStruct_022d8fec_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d8fec_ctl2;

typedef struct UnkStruct_022d8fec_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d8fec_ctl2 *unk8;
} UnkStruct_022d8fec_ctl;

typedef struct UnkStruct_022d8fec_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d8fec_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x12e - 0x118];
    u16 unk12E;
    u8 pad3[0x170 - 0x130];
} UnkStruct_022d8fec_a;

typedef struct UnkStruct_022d8fec UnkStruct_022d8fec;
struct UnkStruct_022d8fec {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d8fec *s);
    u8 pad2[0x13e - 0x6c];
    u16 unk13E;
    u8 pad2a[0x16a - 0x140];
    u16 unk16A;
    u8 pad2c[0x20c - 0x16c];
    s32 unk20C;
    s32 unk210;
    s32 unk214;
    s32 unk218;
    s32 unk21C;
    s32 unk220;
    u16 unk224;
    u8 pad2b[0x228 - 0x226];
    u8 unk228[0x24c - 0x228];
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    u16 unk258;
    u16 unk25A;
    u16 unk25C;
    u16 unk25E;
    u8 pad3[0x264 - 0x260];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d8fec_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d8fec_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d8fec_v;

typedef struct UnkStruct_022d8fec_b {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022d8fec_b;

typedef struct UnkTask_022d8fec {
    u8 pad[0x10];
    UnkStruct_022d8fec *unk10;
} UnkTask_022d8fec;

extern UnkTask_022d8fec *gTaskCurrent;
extern UnkStruct_022d8fec_b *data_ov000_02369b14;
extern s16 data_0206e470[];

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void MTX_RotY33_(void *m, s32 s, s32 c);
void Model_Animate(UnkStruct_022d8fec_a *a);
void Model_Draw(UnkStruct_022d8fec_a *a);
void func_02025268(s32 a, s32 *b, s32 c);
void func_02025a94(void *p, s32 a);

void func_ov000_022d8fec(void) {
    UnkStruct_022d8fec *s = gTaskCurrent->unk10;
    UnkStruct_022d8fec_a *a = &s->unk2C4;
    u16 ang;
    s32 k;
    s32 k2;
    UnkStruct_022d8fec_v saved;
    if (!(data_ov000_02369b14->unk20C & 2)) {
        if (s->unk10 < 0x4000)
            s->unk10 += 0x5fe43;
        else if (s->unk10 > 0x63e43)
            s->unk10 -= 0x5fe43;
        func_ov000_022c5e80(0xf42f8, 0x5fe435, s->unk10 << 4, &a->unk48, &a->unk50, &ang);
        a->unk4C = -s->unk14 << 4;
        {
            u16 r;
            if (s->unk5C & 1)
                r = 0xc000;
            else
                r = 0x4000;
            r += ang;
            MTX_RotY33_(a->unk24, data_0206e470[((r & 0xffff) >> 4) * 2], data_0206e470[((r & 0xffff) >> 4) * 2 + 1]);
        }
    }
    if (s->unk5C & 0x20)
        return;
    saved = *(UnkStruct_022d8fec_v *)&a->unk48;
    a->unk48 += s->unk20C;
    a->unk4C += s->unk210;
    a->unk50 += s->unk214;
    Model_Animate(a);
    Model_Draw(a);
    s->unk214 = 0;
    s->unk210 = s->unk214;
    s->unk20C = s->unk210;
    *(UnkStruct_022d8fec_v *)&a->unk48 = saved;
    k = 2;
    func_02025268(0x10, &k, 1);
    k2 = 0x1e;
    func_02025268(0x14, &k2, 1);
    func_02025a94(s->unk228, 0);
}
