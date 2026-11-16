#include <nitro/types.h>

typedef struct UnkStruct_022c9b00_6 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022c9b00_6;

typedef struct UnkStruct_022c9b00_5 {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022c9b00_6 *unk8;
} UnkStruct_022c9b00_5;

typedef struct UnkStruct_022c9b00_3 {
    u8 pad[0x3f0];
    UnkStruct_022c9b00_5 *unk3F0;
} UnkStruct_022c9b00_3;

typedef struct UnkStruct_022c9b00_2 {
    u8 pad[0x214];
    UnkStruct_022c9b00_3 *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022c9b00_2;

typedef struct UnkStruct_022c9b00 UnkStruct_022c9b00;
struct UnkStruct_022c9b00 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad0[0x68 - 0x30];
    void (*func)(UnkStruct_022c9b00 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x30c - 0x20e];
    UnkStruct_022c9b00_5 *unk30C;
};

extern UnkStruct_022c9b00_2 *gBoss1;

void func_ov000_022ca00c(UnkStruct_022c9b00 *s);
void Boss1Neck_State_Unk_ov000_022c9a6c(UnkStruct_022c9b00 *s);

void Boss1Neck_State_Unk_ov000_022c9b00(UnkStruct_022c9b00 *s) {
    UnkStruct_022c9b00_3 *p = gBoss1->unk214;
    UnkStruct_022c9b00_5 *q;
    s32 k, r, n;
    s32 k2, r2, n2;
    func_ov000_022ca00c(s);
    s->unk20C += gBoss1->unk22A;
    if ((s32)s->unk20C >= 0x1000) {
        s->unk2E = 0;
        s->unk20C = 0x1000;
        s->func = Boss1Neck_State_Unk_ov000_022c9a6c;
    }
    q = s->unk30C;
    k = (s16)s->unk20C;
    n = 1;
    r = q->unk8->unk4 << 12;
    do {
        r = (s32)(((s64)r * k + 0x800) >> 12);
    } while (n-- != 0);
    q->unk0 = r;
    q = p->unk3F0;
    k2 = (s16)s->unk20C;
    n2 = 1;
    r2 = q->unk8->unk4 << 12;
    do {
        r2 = (s32)(((s64)r2 * k2 + 0x800) >> 12);
    } while (n2-- != 0);
    q->unk0 = r2;
}
