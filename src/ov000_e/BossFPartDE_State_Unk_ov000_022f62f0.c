#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    u16 unk210;
    u8 pad212[0x218 - 0x212];
    s32 unk218;
    u8 pad21C[0x35C - 0x21C];
    s32 unk35C;
    u8 pad360[0x4AC - 0x360];
    s16 unk4AC;
    u8 pad4AE[0x4B4 - 0x4AE];
    s32 unk4B4;
    s32 unk4B8;
    s16 unk4BC;
} UnkStruct_022f62f0;

void func_ov000_022f685c(UnkStruct_022f62f0 *w);
void func_ov000_022f62bc(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp(s32 a, s32 b, s32 t) {
    return a + FX_Mul(b - a, t);
}

void BossFPartDE_State_Unk_ov000_022f62f0(UnkStruct_022f62f0 *w) {
    func_ov000_022f685c(w);
    w->unk210 += 0x800;
    w->unk4AC += w->unk4BC;
    if (w->unk4AC >= 0x1000) {
        w->unk4AC = 0x1000;
        w->base.unk68 = func_ov000_022f62bc;
    }
    w->unk218 = Lerp((w->base.unk10 - 0x8000) << 4, (w->unk4B4 - (w->unk4B8 >> 1)) << 4, w->unk4AC) >> 4;
    w->unk35C = Lerp((w->base.unk10 + 0x8000) << 4, (w->unk4B4 + (w->unk4B8 >> 1)) << 4, w->unk4AC) >> 4;
}
