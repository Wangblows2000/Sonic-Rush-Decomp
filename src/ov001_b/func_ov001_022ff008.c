#include <nitro/types.h>

typedef struct UnkStruct_022ff008 {
    u32 unk0;
    s32 unk4;
    u8 pad8[0x10];
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u8 pad22[0x3c - 0x22];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    u8 pad50[0x10];
    s8 unk60[0x314 - 0x60];
    s16 unk314;
    u16 unk316;
} UnkStruct_022ff008;

typedef struct UnkStruct_022ff008_4680 {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022ff008_4680;

extern u32 gSystemFlags;
extern u16 gPad[];
extern UnkStruct_022ff008 *gExtraBossWork;
extern UnkStruct_022ff008_4680 gSaveData;

void PauseMenu_Create(s32 a);
u16 func_ov001_022fee88(u16 k);
void func_ov001_022fea98(void);
s32 FX_ModS32(s32 n, s32 d);
s32 FX_DivS32(s32 n, s32 d);
void func_ov001_02301d14(void);
void Sound_PlaySeqArc(u32 a, u32 b, u32 c);
void func_ov001_02304e80(void);
void func_ov001_022f7224(s32 a);
void func_ov001_02300db4(void);
void func_ov001_02302658(void);
void func_ov001_02304410(void);
void func_ov001_023063a0(void);
void func_ov001_02300214(void);
void func_0204e0c0(void);
void func_ov001_023010e0(void);
void func_ov001_0230109c(void);
void func_ov001_02300fe8(void);
void func_ov001_02303c54(void);
void ExtraBoss_LoadPlayerModel(s32 idx);
void func_ov001_022fecb0(void);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, void *e);
void func_ov001_022f9e58(s32 a, s32 b);
void func_02048864(void);

#define G gExtraBossWork

void func_ov001_022ff008(void) {
    u32 f;
    s32 t;

    if (gSystemFlags & 0x80000000)
        return;
    if ((gPad[2] & 8) && !(G->unk0 & 8)) {
        PauseMenu_Create(0);
        return;
    }
    G->unk18 = func_ov001_022fee88(gPad[0]);
    G->unk1A = func_ov001_022fee88(gPad[1]);
    G->unk1C = func_ov001_022fee88(gPad[2]);
    G->unk1E = func_ov001_022fee88(gPad[3]);
    G->unk20 = func_ov001_022fee88(gPad[4]);
    func_ov001_022fea98();
    if (!(G->unk0 & 2)) {
        G->unk44++;
        if (!(G->unk0 & 0x10) && FX_ModS32(G->unk44, 0x3c) == 0) {
            G->unk4C--;
            if (G->unk4C < 0) {
                func_ov001_02301d14();
                G->unk0 |= 0x10;
                G->unk314 = 0x3c;
                Sound_PlaySeqArc(0, 0, 6);
            }
        }
        if (!(G->unk0 & 0x10) && !(gSaveData.unk8 & 2) && G->unk44 >= 0x8c9f) {
            func_ov001_02301d14();
            G->unk0 |= 0x210;
            G->unk314 = 0x3c;
            G->unk48 = 0;
            Sound_PlaySeqArc(0, 0, 6);
        }
    }
    func_ov001_02304e80();
    func_ov001_022f7224(G->unk4);
    func_ov001_02300db4();
    func_ov001_02302658();
    func_ov001_02304410();
    f = G->unk0;
    if (f & 0x20) {
        if (G->unk316 == 0)
            return;
        G->unk60[G->unk3C]--;
        func_ov001_023063a0();
        if (G->unk60[G->unk3C] < 0) {
            func_ov001_02300214();
            func_0204e0c0();
            return;
        }
        G->unk40 = FX_DivS32(G->unk40, 2) * 2;
        func_ov001_023010e0();
        func_ov001_0230109c();
        func_ov001_02300fe8();
        func_ov001_02303c54();
        ExtraBoss_LoadPlayerModel(0);
        G->unk3C = 0;
        func_ov001_022fecb0();
        G->unk44 = G->unk48;
    } else if (f & 0x10) {
        if (G->unk314 > 0) {
            G->unk314--;
            return;
        }
        G->unk0 = f | 0x20;
        G->unk316 = 0;
        BrightnessLerp_Create(0x10, 0, -0x10, 3, &G->unk316);
    } else if (f & 0x100) {
        t = G->unk44;
        if (f & 0x200)
            t = 0x8c9f;
        func_ov001_022f9e58(t, G->unk4C);
        func_ov001_02300214();
        func_02048864();
    }
}
