#include <nitro/types.h>

typedef struct UnkStruct_022f8c78 {
    u8 pad0[0xe38];
    u32 flags;
    u8 padE3C[0xe48 - 0xe3c];
    s16 unkE48;
} UnkStruct_022f8c78;

typedef struct UnkStruct_022f8c78_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022f8c78_t;

extern UnkStruct_022f8c78 *data_ov001_0231f084;
extern vu8 data_0208923c[];
extern UnkStruct_022f8c78_t *gTaskCurrent;

void func_ov001_022f8a98(void);
void func_ov001_022f8b04(void);

#define REG_DISPCNT (*(vu32 *)0x4000000)

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline void SetVisible(u32 m) {
    u32 v = (REG_DISPCNT & 0x1f00) >> 8;
    REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((v | m) << 8);
}

void func_ov001_022f8c78(void) {
    vu8 *w = data_0208923c;
    s32 s;
    s32 x;
    s32 i;
    u8 v;

    if (data_ov001_0231f084->flags & 3) {
        w[1] = 0;
        w[5] = 0x20;
        w[0] = 0x80;
        w[4] = 0xa1;
        w[3] = 0x7f;
        w[7] = 0x20;
        w[2] = 0;
        w[6] = 0xa1;
        SetVisible(2);
        gTaskCurrent->fn = func_ov001_022f8a98;
        data_ov001_0231f084->flags &= ~0x40;
        data_ov001_0231f084->flags |= 0x80;
        return;
    }
    data_ov001_0231f084->unkE48 += 0x88;
    s = data_ov001_0231f084->unkE48;
    if (s < 0x1000) {
        i = 1;
        x = 0x20;
        do {
            x = FX_Mul(x, s);
        } while (i-- != 0);
        v = x;
        w[5] = v;
        w[4] = 0xc0 - v;
        w[7] = v;
        w[6] = 0xc0 - v;
        if (data_ov001_0231f084->flags & 8)
            SetVisible(2);
    } else {
        w[1] = 0;
        w[5] = 0x20;
        w[0] = 0x80;
        w[4] = 0xa1;
        w[3] = 0x7f;
        w[7] = 0x20;
        w[2] = 0;
        w[6] = 0xa1;
        gTaskCurrent->fn = func_ov001_022f8b04;
        data_ov001_0231f084->unkE48 = 0;
        data_ov001_0231f084->flags |= 0x40;
    }
    data_ov001_0231f084->flags |= 8;
}
