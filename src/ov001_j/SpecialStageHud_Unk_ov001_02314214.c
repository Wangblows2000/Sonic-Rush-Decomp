#pragma opt_lifetimes off
#include <nitro/types.h>

typedef struct UnkStruct_02314214_t {
    u8 pad0[0x14];
    s32 start;
    s32 end;
} UnkStruct_02314214_t;

typedef struct UnkStruct_02314214 {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[4];
    UnkStruct_02314214_t *t;
} UnkStruct_02314214;

extern UnkStruct_02314214 *gSpecialStageData;

s32 SpecialStageMap_GetScrollDistance(void);
s32 *SpecialStageMap_GetCurrentSegment(void);
s32 FX_Div(s32 n, s32 d);
BOOL _fleq(float a, float b);

s32 SpecialStageHud_Unk_ov001_02314214(void) {
    UnkStruct_02314214 *g = gSpecialStageData;
    float v;
    s32 n;
    s32 r;

    v = (float)g->unkC + (float)FX_Div(SpecialStageMap_GetScrollDistance(), 0x12c000) / 4096.0f;
    if (_fleq(v, 2.0f))
        return 0;
    if (*SpecialStageMap_GetCurrentSegment() == 5)
        return 0x800;
    if (*SpecialStageMap_GetCurrentSegment() == 6)
        return 0x1000;
    {
        UnkStruct_02314214_t *t = g->t;
        n = t->start;
        if (_fleq(v, n - 1))
            return 2048.0f * (v - 2.0f) / (n - 5);
        n = t->start;
        r = (s32)(2048.0f * (v - (n + 3)) / (t->end - n - 6)) + 0x800;
    }
    if (r > 0x1000)
        r = 0x1000;
    return r;
}
