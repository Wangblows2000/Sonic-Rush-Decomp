#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0xC0];
    s32 unkC0;
} UnkStruct_0236a444;

extern UnkStruct_0236a444 data_ov000_0236a444;

s32 FX_Div(s32 a, s32 b);
void BossFPartD8_State_Unk_ov000_022f0478(void);

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void BossFPartD8_State_Unk_ov000_022f052c(UnkStruct_StageObj *w) {
    s16 t;

    w->unk32 = 0x2800;
    if (w->unk18 >= -0x12c00) {
        w->unk18 = -0x12c00;
        w->unk68 = BossFPartD8_State_Unk_ov000_022f0478;
    }
    t = FX_Div((0x9c4 + (w->unk18 >> 8)) << 12, 0x898000);
    data_ov000_0236a444.unkC0 = Mul64(t, -0x12c000) + 0x258000;
}
