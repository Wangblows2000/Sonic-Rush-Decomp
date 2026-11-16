#include <nitro/types.h>

typedef struct UnkStruct_022f7924_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_022f7924_o;

typedef struct UnkStruct_022f7924 {
    u8 pad0[0x18];
    UnkStruct_022f7924_o objs[0x17];
    u8 pad690[0xe38 - 0x690];
    u32 unkE38;
    u8 padE3C[0xe76 - 0xe3c];
    s16 unkE76[6];
    u16 digits[6];
    u8 padE8E[2];
    s32 unkE90;
    u8 padE94[0xea0 - 0xe94];
    void *unkEA0;
} UnkStruct_022f7924;

typedef struct UnkStruct_022f7924_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022f7924_t;

extern UnkStruct_022f7924 *data_ov001_0231f084;
extern UnkStruct_022f7924_t *gTaskCurrent;
extern u32 gRandSeed;

void func_ov001_022f7880(void);
void func_ov001_022f7790(void);
void NNS_SndPlayerStopSeq(void **pp, s32 a);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
s32 FX_DivS32(s32 n, s32 d);
void AnimatorSprite_Draw(void *p);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov001_022f7924(void) {
    s32 n;
    s32 i;
    s32 lim;
    s16 x;
    UnkStruct_022f7924_o *o;

    if (data_ov001_0231f084->unkE38 & 3) {
        gTaskCurrent->fn = func_ov001_022f7880;
        func_ov001_022f7880();
        data_ov001_0231f084->unkE38 |= 0x4000;
        NNS_SndPlayerStopSeq(&data_ov001_0231f084->unkEA0, 0);
        Sound_PlaySeqArc(0, 8, 2);
        return;
    }
    data_ov001_0231f084->unkE90++;
    n = FX_DivS32(data_ov001_0231f084->unkE90, 0x10) + 1;
    if (data_ov001_0231f084->unkE90 & 1) {
        for (i = 0; i < 6; i++)
            data_ov001_0231f084->digits[i] = Rand() % 10;
    }
    o = &data_ov001_0231f084->objs[0x16];
    o->x = 0x56;
    o->y = 0x7c;
    AnimatorSprite_Draw(o);
    lim = 6 - n;
    i = 0;
    x = 0x60;
    for (; i < lim; i++, x += 10) {
        o = &data_ov001_0231f084->objs[data_ov001_0231f084->digits[i] + 10];
        o->x = x;
        o->y = 0x7c;
        AnimatorSprite_Draw(o);
    }
    for (; i < 6; i++, x += 10) {
        if (data_ov001_0231f084->unkE76[i] != -1) {
            o = &data_ov001_0231f084->objs[data_ov001_0231f084->unkE76[i] + 10];
            o->x = x;
            o->y = 0x7c;
            AnimatorSprite_Draw(o);
        }
    }
    if (n >= 6) {
        gTaskCurrent->fn = func_ov001_022f7880;
        data_ov001_0231f084->unkE38 |= 1;
        data_ov001_0231f084->unkE38 |= 0x4000;
        NNS_SndPlayerStopSeq(&data_ov001_0231f084->unkEA0, 0);
        Sound_PlaySeqArc(0, 8, 2);
    }
    func_ov001_022f7790();
}
