#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0xC0];
    s32 unkC0;
} UnkStruct_0236a444;

extern UnkStruct_0236a444 data_ov000_0236a444;

s32 func_0200225c(s32 a, s32 b);
void func_ov000_022f0814(void);

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void func_ov000_022f08c8(UnkStruct_StageObj *w) {
    s16 t;

    w->unk32 = 0x2000;
    if (w->unk18 >= -0x12c00) {
        w->unk18 = -0x12c00;
        w->unk68 = func_ov000_022f0814;
    }
    t = func_0200225c(((w->unk18 >> 8) + 0x7d0) << 12, 0x6a4000);
    data_ov000_0236a444.unkC0 = Mul64(t, -0x12c000) + 0x258000;
}
