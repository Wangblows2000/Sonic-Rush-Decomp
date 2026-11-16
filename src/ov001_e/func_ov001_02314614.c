#include <nitro/types.h>

typedef struct UnkStruct_02314614_e {
    u8 pad0[0x20];
    u16 unk20;
} UnkStruct_02314614_e;

UnkStruct_02314614_e *func_ov001_02314664(s32 idx);

s32 func_ov001_02314614(s32 idx, s32 n) {
    UnkStruct_02314614_e *e = func_ov001_02314664(idx);
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
