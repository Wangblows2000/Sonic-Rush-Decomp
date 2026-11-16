#include <nitro/types.h>

typedef struct UnkStruct_02314664_e {
    u8 pad0[0x28];
} UnkStruct_02314664_e;

typedef struct UnkStruct_02314664 {
    u8 pad0[0x10c];
    UnkStruct_02314664_e *entries;
    s32 count;
} UnkStruct_02314664;

extern UnkStruct_02314664 *gSpecialStageTrick;

UnkStruct_02314664_e *SpecialStageTrick_GetSequence(s32 idx) {
    UnkStruct_02314664 *g = gSpecialStageTrick;

    if (idx < 0 && idx >= g->count)
        idx = 0;
    return &g->entries[idx];
}
