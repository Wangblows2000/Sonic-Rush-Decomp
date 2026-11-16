#include <nitro/types.h>

typedef struct UnkStruct_022c6ac4_b {
    u8 pad0[0x48];
    s16 brightness;
} UnkStruct_022c6ac4_b;

typedef struct UnkStruct_022c6ac4 {
    u8 pad0[0x690];
    s32 unk690;
} UnkStruct_022c6ac4;

extern UnkStruct_022c6ac4_b gDisplayMain;
extern UnkStruct_022c6ac4_b gDisplaySub;

BOOL func_0202e5e4(float a, float b);
BOOL func_0202e4b4(float a, float b);

BOOL func_ov001_022c6ac4(UnkStruct_022c6ac4 *w, s32 num, s32 den, BOOL main, BOOL sub) {
    float t = (float)num / (float)den;
    float r;

    if (w->unk690 != 0)
        return FALSE;
    if (func_0202e5e4(t, 1.0f))
        t = 1.0f;
    r = 1.0f - t * t;
    if (main) {
        gDisplayMain.brightness = 16.0f * r;
        if (gDisplayMain.brightness < 0)
            gDisplayMain.brightness = 0;
    }
    if (sub) {
        gDisplaySub.brightness = 16.0f * r;
        if (gDisplaySub.brightness < 0)
            gDisplaySub.brightness = 0;
    }
    return func_0202e4b4(0.0f, r) != 0;
}
