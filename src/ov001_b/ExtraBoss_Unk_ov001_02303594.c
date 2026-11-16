#include <nitro/types.h>

typedef struct UnkStruct_02303594_p UnkStruct_02303594_p;
struct UnkStruct_02303594_p {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[4];
    s32 unk20;
    u8 pad24[4];
    UnkStruct_02303594_p *unk28;
};

typedef struct UnkStruct_02303594_g {
    u8 pad0[0x98];
    UnkStruct_02303594_p *unk98;
    u8 pad9C[4];
    UnkStruct_02303594_p *unkA0;
    u8 padA4[4];
    UnkStruct_02303594_p *unkA8;
    u8 padAC[4];
    UnkStruct_02303594_p *unkB0;
} UnkStruct_02303594_g;

typedef struct UnkStruct_02303594_q {
    u8 pad0[0x310];
    s32 unk310;
    u8 pad314[8];
    s32 unk31C;
} UnkStruct_02303594_q;

extern UnkStruct_02303594_g *data_ov001_0231f548;
extern s16 data_0206e470[];

UnkStruct_02303594_q *ExtraBoss_GetWork(void);
u16 FX_Atan2(s32 y, s32 x);
void ExtraBoss_Unk_ov001_02303394(UnkStruct_02303594_p *p);
void ExtraBoss_Unk_ov001_023031e4(UnkStruct_02303594_p *p);
BOOL ExtraBoss_Unk_ov001_02303554(UnkStruct_02303594_p *p);
void ExtraBoss_StackPush(UnkStruct_02303594_p *p);
void ExtraBoss_Unk_ov001_02303244(UnkStruct_02303594_p *p);
void ExtraBoss_Unk_ov001_0230319c(UnkStruct_02303594_p *p);
void ExtraBoss_Unk_ov001_02303334(UnkStruct_02303594_p *p);
void ExtraBoss_Unk_ov001_023032ec(UnkStruct_02303594_p *p);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBoss_Unk_ov001_02303594(void) {
    s32 tz;
    u16 a;
    UnkStruct_02303594_p *p;
    UnkStruct_02303594_p *n;
    UnkStruct_02303594_p *n2;
    UnkStruct_02303594_p *p2;
    UnkStruct_02303594_p *p3;
    UnkStruct_02303594_p *n3;
    UnkStruct_02303594_p *p4;

    tz = ExtraBoss_GetWork()->unk310;
    n = data_ov001_0231f548->unk98;
    while (n != NULL) {
        p = n;
        n = p->unk28;
        p->unk0 -= 0x4000;
        if (p->unk0 < ExtraBoss_GetWork()->unk31C + 0x80000) {
            p->unk18--;
            if (p->unk18 <= 0) {
                p->unk10 = p->unk14 = 0;
                a = FX_Atan2(tz - p->unk4, 0xc0000 - p->unk0);
                p->unk8 = FX_Mul(COS(a), 0x100);
                p->unkC = FX_Mul(SIN(a), 0x100);
                ExtraBoss_Unk_ov001_02303394(p);
                ExtraBoss_Unk_ov001_023031e4(p);
                continue;
            }
        }
        if (ExtraBoss_Unk_ov001_02303554(p)) {
            ExtraBoss_Unk_ov001_02303394(p);
            ExtraBoss_StackPush(p);
        }
    }
    n2 = data_ov001_0231f548->unkA8;
    while (n2 != NULL) {
        p2 = n2;
        n2 = p2->unk28;
        p2->unk10 += p2->unk8;
        p2->unk14 += p2->unkC;
        p2->unk0 += p2->unk10;
        p2->unk4 += p2->unk14;
        p2->unk18--;
        if (p2->unk18 <= 0) {
            p2->unk10 = p2->unk14 = 0;
            a = FX_Atan2(tz - p2->unk4, 0xc0000 - p2->unk0);
            p2->unk8 = FX_Mul(COS(a), 0x100);
            p2->unkC = FX_Mul(SIN(a), 0x100);
            ExtraBoss_Unk_ov001_02303244(p2);
            ExtraBoss_Unk_ov001_023031e4(p2);
        } else if (ExtraBoss_Unk_ov001_02303554(p2)) {
            ExtraBoss_Unk_ov001_02303244(p2);
            ExtraBoss_StackPush(p2);
        }
    }
    n3 = data_ov001_0231f548->unkB0;
    while (n3 != NULL) {
        p3 = n3;
        n3 = p3->unk28;
        p3->unk10 += p3->unk8;
        p3->unk14 += p3->unkC;
        p3->unk0 += p3->unk10;
        p3->unk4 += p3->unk14;
        if (p3->unk0 <= 0x11a000 && p3->unk0 >= 0x66000 && p3->unk4 <= tz + 0x10000 && p3->unk4 >= tz - 0x10000) {
            ExtraBoss_Unk_ov001_0230319c(p3);
            ExtraBoss_Unk_ov001_02303334(p3);
            p3->unk20 = 0xf;
        }
    }
    p4 = data_ov001_0231f548->unkA0;
    while (p4 != NULL) {
        p4->unk20--;
        if (p4->unk20 < 0) {
            ExtraBoss_Unk_ov001_023032ec(p4);
            ExtraBoss_StackPush(p4);
        }
        p4 = p4->unk28;
    }
}
