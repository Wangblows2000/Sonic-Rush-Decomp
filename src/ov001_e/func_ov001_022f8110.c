#include <nitro/types.h>

typedef struct UnkStruct_022f8110 {
    u8 pad0[0xe1c];
    void *unkE1C;
    u8 padE20[4];
    s32 unkE24;
    s32 unkE28;
    s32 unkE2C;
    u8 padE30[0xe5e - 0xe30];
    s16 dig[3][6];
} UnkStruct_022f8110;

extern UnkStruct_022f8110 *data_ov001_0231f084;

s32 func_02002094(s32 n, s32 d);
s32 func_0200203c(s32 n, s32 d);
void *func_020372d0(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void func_ov001_022f7e20(void);

static inline void SetDigits(s32 v, s32 k) {
    s32 i;
    s32 seen;
    s32 div;
    s32 d;

    i = 0;
    seen = 0;
    div = 100000;
    for (; i < 6; i++) {
        d = (s16)func_02002094(v, div);
        if (d == 0) {
            if (!seen && i < 5)
                d = -1;
        } else {
            seen = 1;
            d = (s16)d;
        }
        data_ov001_0231f084->dig[k][i] = d;
        v = func_0200203c(v, div);
        div /= 10;
    }
}

static inline void SetDigits0(s32 k) {
    s32 i;
    s32 v;
    s32 seen;
    s32 div;
    s32 d;

    i = 0;
    seen = 0;
    v = data_ov001_0231f084->unkE24;
    div = 100000;
    for (; i < 6; i++) {
        d = (s16)func_02002094(v, div);
        if (d == 0) {
            if (!seen && i < 5)
                d = -1;
        } else {
            seen = 1;
            d = (s16)d;
        }
        data_ov001_0231f084->dig[k][i] = d;
        v = func_0200203c(v, div);
        div /= 10;
    }
}

void func_ov001_022f8110(void) {
    SetDigits0(0);
    SetDigits(data_ov001_0231f084->unkE28, 1);
    SetDigits(data_ov001_0231f084->unkE2C, 2);
    data_ov001_0231f084->unkE1C = func_020372d0(func_ov001_022f7e20, 0, 0, 0x2000, 0, 0);
}
