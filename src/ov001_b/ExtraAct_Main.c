#include <nitro/types.h>

typedef struct UnkStruct_022f9188 {
    u8 pad0[0xb58];
    u8 unkB58[0xe0c - 0xb58];
    s32 unkE0C;
    s32 unkE10;
    s32 unkE14;
    s32 unkE18;
    s32 unkE1C;
    s32 unkE20;
    u8 padE24[0xe34 - 0xe24];
    s32 unkE34;
    u32 unkE38;
    u16 unkE3C;
    u8 padE3E[2];
    s32 unkE40;
} UnkStruct_022f9188;

typedef struct UnkStruct_022f9188_k {
    u16 held;
    u16 unk2;
    u16 trig;
} UnkStruct_022f9188_k;

extern UnkStruct_022f9188 *data_ov001_0231f084;
extern UnkStruct_022f9188_k gPad;

void Model_Draw(void *p);
void ExtraAct_Unk_ov001_022f9134(void);
void ExtraAct_Unk_ov001_022f8e7c(void);
void ExtraAct_Unk_ov001_022f8a40(void);
void ExtraAct_Unk_ov001_022f8774(void);
void ExtraAct_Unk_ov001_022f8110(void);
void ExtraAct_Unk_ov001_022f7740(void);
void ExtraAct_ReleaseAssets(void);
void SceneChange_ToExtraEnding(void);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, void *e);

#define G data_ov001_0231f084

void ExtraAct_Main(void) {
    Model_Draw(G->unkB58);
    if (gPad.trig & 1) {
        if (!(G->unkE38 & 1)) {
            G->unkE38 |= 1;
            if (!(G->unkE38 & 0x30) && G->unkE0C == 0) {
                ExtraAct_Unk_ov001_022f9134();
                G->unkE38 &= ~0x10;
                G->unkE38 |= 0x20;
            }
            if (!(G->unkE38 & 0x3c0) && G->unkE10 == 0) {
                ExtraAct_Unk_ov001_022f8e7c();
                G->unkE38 &= ~0x40;
                G->unkE38 |= 0x80;
            }
            if (!(G->unkE38 & 0xc00) && G->unkE14 == 0) {
                ExtraAct_Unk_ov001_022f8a40();
                G->unkE38 &= ~0x400;
                G->unkE38 |= 0x800;
            }
            if (!(G->unkE38 & 0x3000) && G->unkE18 == 0) {
                ExtraAct_Unk_ov001_022f8774();
                G->unkE38 &= ~0x1000;
                G->unkE38 |= 0x2000;
            }
            if (!(G->unkE38 & 0xc000) && G->unkE1C == 0) {
                ExtraAct_Unk_ov001_022f8110();
                G->unkE38 &= ~0x4000;
                G->unkE38 |= 0x8000;
            }
        } else if (!(G->unkE38 & 2)) {
            G->unkE38 |= 2;
            if (!(G->unkE38 & 0x30000) && G->unkE20 == 0) {
                ExtraAct_Unk_ov001_022f7740();
                G->unkE38 &= ~0x10000;
                G->unkE38 |= 0x20000;
            }
        }
    }
    if (!(G->unkE38 & 4) && G->unkE3C != 0) {
        G->unkE3C = 0;
        G->unkE38 |= 4;
        ExtraAct_Unk_ov001_022f9134();
    } else if (G->unkE38 & 0x10) {
        G->unkE38 &= ~0x10;
        G->unkE38 |= 0x20;
        ExtraAct_Unk_ov001_022f8e7c();
    } else if (G->unkE38 & 0x40) {
        G->unkE38 &= ~0x40;
        G->unkE38 |= 0x80;
        ExtraAct_Unk_ov001_022f8a40();
    } else if (G->unkE38 & 0x400) {
        G->unkE38 &= ~0x400;
        G->unkE38 |= 0x800;
        ExtraAct_Unk_ov001_022f8774();
    } else if (G->unkE38 & 0x1000) {
        G->unkE38 &= ~0x1000;
        G->unkE38 |= 0x2000;
        ExtraAct_Unk_ov001_022f8110();
    } else if (G->unkE38 & 0x4000) {
        G->unkE38 &= ~0x4000;
        G->unkE38 |= 0x8000;
        ExtraAct_Unk_ov001_022f7740();
    } else if (G->unkE38 & 0x10000) {
        G->unkE38 &= ~0x10000;
        G->unkE38 |= 0x20002;
    } else if (G->unkE38 & 2) {
        if (G->unkE3C != 0) {
            G->unkE40++;
            if (G->unkE40 >= 0x3c) {
                ExtraAct_ReleaseAssets();
                SceneChange_ToExtraEnding();
            }
        } else {
            G->unkE34++;
            if (G->unkE34 == 0x78)
                BrightnessLerp_Create(0x10, 0, 0x10, 3, (u8 *)G + 0xe3c);
        }
    }
}
