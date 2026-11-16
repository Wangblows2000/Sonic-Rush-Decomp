#include <nitro/types.h>

typedef struct UnkMtx22_022c7278 {
    s32 m[4];
} UnkMtx22_022c7278;

typedef struct UnkStruct_022c7278_s {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_022c7278_s;

typedef struct UnkStruct_022c7278 {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x50 - 0x10];
    UnkStruct_022c7278_s *unk50;
    u8 pad54[0x6a4 - 0x54];
    float unk6A4;
    float unk6A8;
    float unk6AC;
} UnkStruct_022c7278;

extern u8 data_02089264[];

s32 FX_Inv(s32 a);
BOOL Runtime_Unk_0202e5e4(float a, float b);
BOOL Runtime_Unk_0202e528(float a, float b);
void MTX_Scale22_(UnkMtx22_022c7278 *m, s32 x, s32 y);
void G2x_SetBGyAffine_(void *p, UnkMtx22_022c7278 *m, s32 x, s32 y, s32 a, s32 b);

void Ending_UpdatePictureZoomShake(UnkStruct_022c7278 *w) {
    UnkMtx22_022c7278 mtx;
    s32 v = 0x2000 - w->unkC * 10;
    float f = (float)FX_Inv(v) / 4096.0f;
    float a;
    float b;

    if (v <= 0x1000) {
        f = 1.0f;
        v = 0x1000;
    }
    a = w->unk6A8 * f;
    b = w->unk6A4 * f;
    MTX_Scale22_(&mtx, v, v);
    G2x_SetBGyAffine_(data_02089264, &mtx, (w->unk50->unk18 << 3) / 2, w->unk50->unk1A << 3, (s32)b, (s32)a);
    if (w->unkC & 0xc) {
        w->unk6A8 = w->unk6A8 + 3.0f;
        if (Runtime_Unk_0202e5e4(w->unk6A8, 10.0f))
            w->unk6A8 = 10.0f;
    } else {
        w->unk6A8 = w->unk6A8 - 3.0f;
        if (Runtime_Unk_0202e528(w->unk6A8, -10.0f))
            w->unk6A8 = -10.0f;
    }
    w->unk6A4 = w->unk6A4 + w->unk6AC;
    if ((w->unkC & 0xf) == 8)
        w->unk6AC = w->unk6AC * -1.0f;
}
