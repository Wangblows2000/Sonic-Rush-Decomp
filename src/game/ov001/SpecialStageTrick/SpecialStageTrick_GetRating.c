#include <nitro/types.h>

typedef struct UnkStruct_02314614_e {
    u8 pad0[0x20];
    u16 unk20;
} UnkStruct_02314614_e;

UnkStruct_02314614_e *SpecialStageTrick_GetSequence(s32 idx);

s32 SpecialStageTrick_GetRating(s32 idx, s32 n) {
    UnkStruct_02314614_e *e = SpecialStageTrick_GetSequence(idx);
    s32 third = e->unk20 / 3;
    s32 d = e->unk20 - n;
    s32 thr = third * 2;
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (thr <= d)
            return i;
        thr -= third;
    }
    return 0;
}
