#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkCfg_02343228;

extern UnkCfg_02343228 gGameState;
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Player_AddLives(u8 *w, s32 a);

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void Player_AddRings(u8 *w, s16 d) {
    s16 old = *(s16 *)(w + 0x3ae);
    s16 t;
    s32 v;

    *(s16 *)(w + 0x3ae) = old + d;
    v = *(s16 *)(w + 0x3ae);
    *(s16 *)(w + 0x3ae) = CLAMP(v, 0, 999);
    *(s16 *)(w + 0x3b0) += d;
    v = *(s16 *)(w + 0x3b0);
    *(s16 *)(w + 0x3b0) = CLAMP(v, 0, 9999);
    Sound_PlaySeqArcEx(0, 0, 4);
    if (gGameState.unk10 - 1U <= 1)
        return;
    for (t = 100; t <= 900; t += 100) {
        if (old < t && *(s16 *)(w + 0x3ae) >= t)
            Player_AddLives(w, 1);
    }
}
