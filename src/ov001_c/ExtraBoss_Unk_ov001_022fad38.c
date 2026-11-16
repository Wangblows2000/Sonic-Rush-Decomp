#include <nitro/types.h>

typedef struct UnkStruct_022fad38_4560 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022fad38_4560;

typedef struct UnkStruct_022fad38_g {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_022fad38_g;

extern u32 gRandSeed;
extern s16 data_0206e470[];
extern UnkStruct_022fad38_4560 gGameState;
extern s32 data_ov001_0231d68c[][5];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

u8 *ExtraBossPlayer_CreateFxObj(s32 kind, void *parent, s32 x, s32 y, s32 z);
void *ExtraBoss_GetPlayer(void);
void ExtraBossObj_State_Unk_ov001_022fac54(void);
UnkStruct_022fad38_g *ExtraBoss_GetWork(void);
s32 FX_DivS32(s32 n, s32 d);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline BOOL IsMode_022fad38(void) {
    BOOL r = TRUE;
    switch (gGameState.unk10) {
    case 2:
    case 3:
        break;
    default:
        r = FALSE;
        break;
    }
    return r;
}

void ExtraBoss_Unk_ov001_022fad38(void *parent, s32 x, s32 y, s32 z, u16 ang) {
    u8 *t;
    s32 r;
    s32 idx;
    s32 k;

    r = Rand() & 0xff;
    t = ExtraBossPlayer_CreateFxObj(0xe, parent, x, y, z);
    if (t == NULL)
        return;
    *(u32 *)(t + 0x14) &= ~1;
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u32 *)(t + 0x14) |= 0x10;
    idx = (s32)(ang & 0xffff) >> 4;
    *(void **)(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fac54;
    *(s32 *)(t + 0x58) = -0x2000;
    *(s32 *)(t + 0x64) = 0x88;
    *(s32 *)(t + 0x50) = FX_Mul(COS_IDX(idx), 0x8000);
    *(s32 *)(t + 0x54) = FX_Mul(SIN_IDX(idx), 0x8000);
    *(s32 *)(t + 0x1d4) = *(s32 *)(t + 0x50);
    *(s32 *)(t + 0x1d8) = *(s32 *)(t + 0x54);
    if (IsMode_022fad38())
        k = 1;
    else
        k = gGameState.unk14;
    *(s32 *)(t + 0x1d0) = FX_DivS32(r * data_ov001_0231d68c[k][ExtraBoss_GetWork()->unk40] + ((0x100 - r) << 12), 0x100);
    *(s16 *)(t + 0x1cc) = 0;
    Sound_PlaySeqArc(0, 0, 0xb0);
}
