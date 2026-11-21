#include <nitro/types.h>

typedef struct UnkStruct_022c69ac_b {
    u8 pad0[0x48];
    s16 brightness;
} UnkStruct_022c69ac_b;

typedef struct UnkStruct_022c69ac {
    u8 pad0[0x690];
    s32 unk690;
} UnkStruct_022c69ac;

extern UnkStruct_022c69ac_b gDisplayMain;
extern UnkStruct_022c69ac_b gDisplaySub;

BOOL _fgr(float a, float b);
BOOL _feq(float a, float b);

BOOL Ending_FadeOut(UnkStruct_022c69ac *w, s32 num, s32 den, BOOL main, BOOL sub) {
    float t = (float)num / (float)den;
    float r;

    if (w->unk690 != 0)
        return FALSE;
    if (_fgr(t, 1.0f))
        t = 1.0f;
    r = 1.0f - t;
    r = 1.0f - r * r;
    if (main) {
        gDisplayMain.brightness = 16.0f * r;
        if (gDisplayMain.brightness > 16)
            gDisplayMain.brightness = 16;
    }
    if (sub) {
        gDisplaySub.brightness = 16.0f * r;
        if (gDisplaySub.brightness > 16)
            gDisplaySub.brightness = 16;
    }
    return _feq(1.0f, r) != 0;
}
