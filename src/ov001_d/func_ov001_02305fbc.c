#include <nitro/types.h>

typedef struct UnkStruct_02305fbc_w {
    u8 pad0[4];
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 pad8[4];
    u32 unkC;
} UnkStruct_02305fbc_w;

typedef struct UnkStruct_02305fbc {
    u8 pad0[8];
    void *unk8;
    u8 padC[0x5e4 - 0xc];
    s16 unk5E4;
} UnkStruct_02305fbc;

extern UnkStruct_02305fbc *data_ov001_0231f5a8;
extern UnkStruct_02305fbc_w data_02087d14;
extern void *gTaskCurrent;

void Task_Destroy(void *t);
void func_ov001_02305ee8(void);
void func_ov001_02305c84(void);

static inline u32 GetPlanes(void) {
    return (*(vu32 *)0x4001000 & 0x1f00) >> 8;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_02305fbc(void) {
    UnkStruct_02305fbc_w *w = &data_02087d14;
    s32 v;
    s32 acc;
    s32 i;
    s32 v2;
    s32 acc2;
    s32 i2;

    data_ov001_0231f5a8->unk5E4 += 0x199;
    v = data_ov001_0231f5a8->unk5E4;
    if (v < 0x1000) {
        i = 3;
        acc = 0x56;
        do {
            acc += FX_Mul(0 - acc, v);
        } while (i-- != 0);
        w->unk7 = acc;
        w->unk5 = w->unk7;
        i2 = 3;
        acc2 = 0x60;
        v2 = data_ov001_0231f5a8->unk5E4;
        do {
            acc2 += FX_Mul(0xc0 - acc2, v2);
        } while (i2-- != 0);
        w->unk6 = acc2;
        w->unk4 = w->unk6;
        return;
    }
    Task_Destroy(gTaskCurrent);
    data_ov001_0231f5a8->unk8 = NULL;
    w->unkC = 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | ((GetPlanes() & ~1) << 8);
    func_ov001_02305ee8();
    func_ov001_02305c84();
}
