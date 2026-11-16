#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x440 - 0x20C];
    s32 unk440;
    s32 unk444;
    s32 unk448;
    s16 unk44C;
    u8 pad44E[2];
    u8 unk450[4];
} UnkStruct_022f065c;

void BossF_UpdateCameraShake(void *p, s32 a, s32 b);
void func_ov000_022f0fdc(UnkStruct_022f065c *w, s32 a);
void func_ov000_022f0640(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022f065c(UnkStruct_022f065c *w) {
    BossF_UpdateCameraShake(w->unk450, 4, 8);
    func_ov000_022f0fdc(w, 4);
    w->unk44C += 0x2d;
    if (w->unk44C >= 0x1000) {
        w->unk44C = 0x1000;
        w->base.unk68 = func_ov000_022f0640;
    }
    w->base.unk10 = (w->unk440 + FX_Mul(0x1a0000 - w->unk440, w->unk44C)) >> 4;
    w->base.unk14 = (w->unk444 + FX_Mul(0x358000 - w->unk444, w->unk44C)) >> 4;
    w->base.unk18 = (w->unk448 + FX_Mul(-0x9c4000 - w->unk448, w->unk44C)) >> 4;
}
