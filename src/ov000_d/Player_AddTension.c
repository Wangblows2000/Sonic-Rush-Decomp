#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_0234307c_4560;

typedef struct {
    u8 pad0[0x325];
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
    u8 pad330[0x350 - 0x330];
    s16 unk350;
    u8 pad352[0x3B6 - 0x352];
    s16 unk3B6;
    u8 pad3B8[0x414 - 0x3B8];
    u8 unk414[4];
} UnkStruct_0234307c;

extern UnkStruct_0234307c_4560 gGameState;

void func_ov000_0232ecac(s32 a, s32 b);
void EffVsTensionMax_Create(UnkStruct_0234307c *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void ModelPalette_ShiftColor(void *p, s32 a, s32 b, s32 c);

void Player_AddTension(UnkStruct_0234307c *w, s16 v) {
    s16 t;

    if ((gGameState.unk18 & 0x20) && v == 0x12c0) {
        if (w->unk325 == 0) {
            func_ov000_0232ecac(0x2f, 0x80);
        }
        EffVsTensionMax_Create(w);
    }
    if (w->unk32C & 0x400000) {
        return;
    }
    if (v < 0) {
        if (w->unk3B6 != 0) {
            Sound_PlaySeqArcEx(NULL, 0, 0x3c);
            ModelPalette_ShiftColor(w->unk414, 0, 0, 0);
        }
        w->unk3B6 = 0;
    }
    w->unk350 += v;
    if (w->unk3B6 == 0) {
        if (w->unk350 >= 0x12c0) {
            w->unk3B6 = 0x140;
            Sound_PlaySeqArcEx(NULL, 0, 0x3a);
        }
    } else if (v > 0) {
        w->unk3B6 = 0x140;
        Sound_PlaySeqArcEx(NULL, 0, 0x3b);
    }
    t = w->unk350;
    if (t < 0) {
        t = 0;
    } else if (t > 0x12c0) {
        t = 0x12c0;
    }
    w->unk350 = t;
}
