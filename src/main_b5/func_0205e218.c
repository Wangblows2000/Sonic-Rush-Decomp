#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    s32 vram;
} UnkSpr_0205e218;

void func_02039bb0(s32 a, s32 b);

static inline void Release_0205e218(UnkSpr_0205e218 *sp) {
    UnkSpr_0205e218 *s = sp;
    if (s->vram != 0) {
        func_02039bb0(1, s->vram);
        s->vram = 0;
    }
}

void func_0205e218(u8 *w) {
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x24));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x8c));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0xe4));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x12c));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x184));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x1f4));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x264));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x2ac));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x328));
    Release_0205e218((UnkSpr_0205e218 *)(w + 0x370));
}
