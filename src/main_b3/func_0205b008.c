#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    s16 brightness;
} UnkDisp_0205b008;

typedef struct {
    u8 pad0[0x474];
    void (*fn)(void);
    u8 pad478[0x488 - 0x478];
    u32 unk488;
    u8 pad48C[0x8ce - 0x48c];
    s16 t;  /* 0x8ce */
} UnkWork_0205b008;
typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern UnkDisp_0205b008 data_02087d04;
extern UnkDisp_0205b008 data_0208922c;

void func_0205aff8(void);

static inline s32 FX_Mul_0205b008(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_0205b008(UnkWork_0205b008 *w) {
    BOOL flag;
    w->t += 0xa3;
    if (w->t >= 0x1000) {
        w->t = 0x1000;
        if (w->unk488 & 0x400000)
            w->fn = func_0205aff8;
    }
    data_02087d04.brightness = FX_Mul_0205b008(0x10000, w->t) >> 12;
    flag = FALSE;
    if (data_022c4560.unk1C == 8 && data_022c4560.unk1D == 2)
        flag = TRUE;
    if (flag)
        return;
    if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)
        return;
    data_0208922c.brightness = FX_Mul_0205b008(0x10000, w->t) >> 12;
}
