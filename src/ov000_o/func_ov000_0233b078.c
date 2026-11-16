#include <nitro/types.h>

typedef struct {
    u16 w;
    u16 h;
    u8 data[1];
} UnkMap_0233b078;

typedef struct {
    u8 pad0[0xe];
    u16 unkE;
} UnkStruct_0236abd8;

extern u8 data_ov000_0236aad8[];
extern UnkStruct_0236abd8 data_ov000_0236abd8;

s32 func_ov000_0233b078(s32 x) {
    UnkMap_0233b078 *m = *(UnkMap_0233b078 **)(data_ov000_0236aad8 + 0x13c);
    u16 h;
    u16 i;
    u16 w;
    u16 start;
    BOOL found = FALSE;
    u32 idx;
    s32 v;
    s32 cx;

    if (x < 0) {
        cx = 0;
    } else {
        cx = (data_ov000_0236abd8.unkE << 8) - 1;
        if (x <= cx)
            cx = x;
    }
    h = m->h;
    w = m->w;
    for (i = 0; i < h; i++) {
        idx = i * w + (cx >> 14);
        v = (m->data[(u16)(idx >> 2)] >> (u16)((idx & 3) << 1)) & 3;
        if (!found && v != 0) {
            if (v == 1)
                return i << 14;
            found = TRUE;
            start = i;
        }
        if (found == TRUE && v == 1)
            return (start + i) << 13;
    }
    return 0;
}
