#include <nitro/types.h>

typedef struct { s32 unk0; u8 pad4[0x18]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

typedef struct {
    s32 from[3];  /* 0x0 */
    u8 padC[0xc];
    s32 unk18;    /* 0x18 */
    u8 pad1C[4];
    s32 unk20;    /* 0x20 */
    u8 pad24[4];
    s32 unk28;    /* 0x28 */
    s32 toB[3];   /* 0x2c */
    s32 unk38;    /* 0x38 */
    s32 toA[3];   /* 0x3c */
    u8 pad48[8];
} UnkTbl_0207346c;
extern UnkTbl_0207346c data_0207346c[];

typedef struct UnkObj_02046a54 {
    void (*fn)(void);
    u8 pad4[0x144];
    s32 unk148[3];
    s16 unk154;
    s16 t;
} UnkObj_02046a54;

void ClearDemoModel_State_Done(void);

#pragma thumb on
static inline s32 FX_Mul_02046a54(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_02046a54(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_02046a54(to - v, t);
    } while (n--);
    return v;
}

void ClearDemoModel_State_Move(void *unused, UnkObj_02046a54 *o) {
    s32 idx = gGameState.unk0;
    if (idx > 1)
        idx = 0;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        o->t += data_0207346c[idx].unk28;
        o->unk148[0] = Lerp_02046a54(data_0207346c[idx].from[0], data_0207346c[idx].toA[0], o->t, 3);
        o->unk148[1] = Lerp_02046a54(data_0207346c[idx].from[1], data_0207346c[idx].toA[1], o->t, 3);
        o->unk148[2] = Lerp_02046a54(data_0207346c[idx].from[2], data_0207346c[idx].toA[2], o->t, 3);
    } else {
        o->t += data_0207346c[idx].unk20;
        o->unk148[0] = Lerp_02046a54(data_0207346c[idx].from[0], data_0207346c[idx].toB[0], o->t, 3);
        o->unk148[1] = Lerp_02046a54(data_0207346c[idx].from[1], data_0207346c[idx].toB[1], o->t, 3);
        o->unk148[2] = Lerp_02046a54(data_0207346c[idx].from[2], data_0207346c[idx].toB[2], o->t, 3);
    }
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = ClearDemoModel_State_Done;
    }
    o->unk154 = Lerp_02046a54(data_0207346c[idx].unk18 << 12, data_0207346c[idx].unk38 << 12, o->t, 2) >> 12;
}
#pragma thumb reset
