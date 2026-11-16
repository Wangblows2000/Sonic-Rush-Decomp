#include <nitro/types.h>

typedef struct UnkStruct_022f7e20_s {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkStruct_022f7e20_s;

typedef struct UnkStruct_022f7e20 {
    u8 pad0[0x18];
    UnkStruct_022f7e20_s spr[0x27];
    u8 padB10[0xe38 - (0x18 + 0x27 * 0x48)];
    u32 unkE38;
    u8 padE3C[0xe5e - 0xe3c];
    s16 unkE5E[2][6];
    u8 padE76[0xe90 - 0xe76];
    s32 unkE90;
    s16 unkE94[2];
    s16 unkE98;
} UnkStruct_022f7e20;

typedef struct UnkStruct_022f7e20_t {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_022f7e20_t;

extern UnkStruct_022f7e20 *data_ov001_0231f084;
extern UnkStruct_022f7e20_t *data_022b4574;
extern s32 data_ov001_02316a80[];

void func_ov001_022f7880(void);
void func_ov001_022f7d64(void);
s32 func_02002094(s32 n, s32 d);
void func_02036754(u32 a, u32 b, u32 c);
void func_02032d2c(void *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

#define G data_ov001_0231f084
#define E38 (G->unkE38)
#define E90 (G->unkE90)
#define E94 (G->unkE94)
#define E98 (G->unkE98)
#define SPR(i) (&G->spr[i])

void func_ov001_022f7e20(void) {
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s16 yy;
    s32 v;
    s32 v2;
    s32 acc2;
    s32 k2;
    s32 acc;
    s32 t;
    UnkStruct_022f7e20_s *s;
    UnkStruct_022f7e20 *g;

    if (E38 & 3) {
        data_022b4574->unk8 = func_ov001_022f7880;
        func_ov001_022f7880();
        E38 |= 0x4000;
        return;
    }
    E90++;
    n = func_02002094(E90, 0x1e) + 1;
    if (n >= 3)
        n = 3;
    for (i = 0; i < n; i++) {
        if (E94[i] != 0x1000) {
            E94[i] += 0x100;
            if (E94[i] >= 0x1000) {
                E94[i] = 0x1000;
                func_02036754(0, 8, 0);
            }
        }
    }
    for (i = 0, yy = 0x54; i <= 1; i++, yy += 0x10) {
        g = G;
        v = g->unkE94[i];
        if (v != 0) {
            k = 5;
            acc = -0xc8;
            do {
                acc += FX_Mul(-acc, v);
            } while (k-- != 0);
            s = &g->spr[data_ov001_02316a80[i]];
            s->x = acc + 0x56;
            s->y = yy;
            func_02032d2c(s);
            acc = (s16)(acc + 0x60);
            for (j = 0; j < 6; j++, acc = (s16)(acc + 10)) {
                t = G->unkE5E[i][j];
                if (t != -1) {
                    s = SPR(t);
                    s->x = acc;
                    s->y = yy;
                    func_02032d2c(s);
                }
            }
        }
    }
    v2 = E98;
    if (v2 != 0) {
        k2 = 5;
        acc2 = -0xc8;
        do {
            acc2 += FX_Mul(-acc2, v2);
        } while (k2-- != 0);
        s = (UnkStruct_022f7e20_s *)((u8 *)G + 0x690);
        s->x = acc2 + 0x96;
        s->y = 0x6c;
        func_02032d2c(s);
    }
    if (E98 >= 0x1000) {
        data_022b4574->unk8 = func_ov001_022f7d64;
        E90 = 0x3c;
    }
}
