#include <nitro/types.h>

typedef struct UnkStruct_022f5a24_a {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10d];
    s32 unk114;
} UnkStruct_022f5a24_a;

typedef struct UnkStruct_022f5a24_s {
    u8 pad0[0x134];
    s32 unk134;
    s32 unk138;
} UnkStruct_022f5a24_s;

typedef struct UnkStruct_022f5a24 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x10];
    s32 unk30;
    u8 pad34[0x74 - 0x34];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f5a24_a *unk84;
    u8 pad88[0x1c8 - 0x88];
    s16 unk1C8;
    u8 pad1CA[2];
    s32 unk1CC;
    s32 unk1D0;
    s16 unk1D4;
    u8 pad1D6[0x1dc - 0x1d6];
    UnkStruct_022f5a24_s *unk1DC;
} UnkStruct_022f5a24;

typedef struct UnkStruct_022f5a24_t {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
} UnkStruct_022f5a24_t;

typedef struct UnkStruct_022f5a24_q {
    u8 pad0[0x308];
    s32 unk308;
    s32 unk30C;
} UnkStruct_022f5a24_q;

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;

void func_ov001_022f5984(void);
void func_ov001_022f5954(void);
void func_ov001_022f5da0(void);
void func_ov001_022f5a24(UnkStruct_022f5a24 *w);
void func_02031a68(void *p, s32 a, void *b, s32 c, u32 d);
UnkStruct_022f5a24_t *func_ov001_02300c94(void);
u16 func_020027a4(s32 y, s32 x);
s32 func_02002094(s32 n, s32 d);
void func_ov001_022fc09c(UnkStruct_022f5a24 *w, s32 x, s32 y, s32 z, u16 ang);
UnkStruct_022f5a24_q *func_ov001_022ffce8(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022f5a24(UnkStruct_022f5a24 *w) {
    UnkStruct_022f5a24_a *a = w->unk84;
    UnkStruct_022f5a24_t *t;
    UnkStruct_022f5a24_s *s;
    UnkStruct_022f5a24_q *q1;
    UnkStruct_022f5a24_q *q2;
    UnkStruct_022f5a24_q *q3;
    u16 ang;
    s32 acc2;
    s32 acc;
    s32 i;
    s32 v2;
    s32 i2;
    s32 v;

    if (w->unk1C8 < 0x1000) {
        w->unk1C8 += 0x199;
        if (w->unk1C8 > 0x1000)
            w->unk1C8 = 0x1000;
    } else {
        w->unk1D4 += 0x44;
        if (w->unk1D4 > 0x1000)
            w->unk1D4 = 0x1000;
    }
    if (w->unk1D4 >= 0x1000) {
        w->unk30 = 0;
        w->unk1CC--;
        if (w->unk1CC <= 0) {
            w->unk74 = func_ov001_022f5984;
            if (*a->unkE4 > 0x64000) {
                a->unk114 = -0x10000;
                return;
            }
            if (*a->unkE4 < 0x64000) {
                a->unk114 = 0x10000;
                return;
            }
            w->unk74 = func_ov001_022f5954;
            func_02031a68(a, 0, data_ov001_0231f068, 7, 0);
            a->unk10C &= ~4;
            a->unk114 = 0x1000;
            return;
        }
        t = func_ov001_02300c94();
        w->unk74 = func_ov001_022f5da0;
        ang = func_020027a4(t->unk1C - w->unk1C, t->unk18 - w->unk18);
        if (ang < 0x5c71)
            ang = 0x5c71;
        else if (ang > 0xb1c7)
            ang = 0xb1c7;
        w->unk1D0 = func_02002094((u16)(ang + 0xa38f), 0xb6) << 13;
        if (*a->unkE4 == w->unk1D0) {
            s = w->unk1DC;
            a->unk114 = 0;
            w->unk74 = (void (*)(void))func_ov001_022f5a24;
            a->unk114 = 0;
            func_ov001_022fc09c(w, s->unk134 - data_ov001_0231f524, -(s->unk138 + data_ov001_0231f520), 0,
                                (((w->unk1D0 >> 13) << 16) / 360) - 0xa38f);
            w->unk1C8 = 0;
            w->unk1D4 = 0;
            return;
        }
        q1 = func_ov001_022ffce8();
        q2 = func_ov001_022ffce8();
        q3 = func_ov001_022ffce8();
        a->unk114 = func_02002094(q1->unk308 * 0xa000 + ((q2->unk30C - q3->unk308) << 16), func_ov001_022ffce8()->unk30C);
        if (w->unk1D0 < *a->unkE4)
            a->unk114 = -a->unk114;
        return;
    }
    if (w->unk1D4 == 0) {
        v = w->unk1C8;
        i = 5;
        acc = 0;
        do {
            acc += FX_Mul(0x18000 - acc, v);
        } while (i-- != 0);
        w->unk30 = acc;
    } else {
        v2 = w->unk1D4;
        i2 = 3;
        acc2 = 0x18000;
        do {
            acc2 += FX_Mul(-acc2, v2);
        } while (i2-- != 0);
        w->unk30 = acc2;
    }
}
