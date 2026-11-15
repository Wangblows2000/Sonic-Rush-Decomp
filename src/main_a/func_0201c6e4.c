#include <nitro/types.h>

BOOL func_0201b584(u32 mask);

typedef struct { u8 unk0[0x44]; u32 mask; s32 count; u8 ids[1]; } UnkStruct_0201c6e4;

BOOL func_0201c6e4(UnkStruct_0201c6e4 *s, s32 count, const u8 *ids) {
    u32 mask = 0;
    s32 i;
    for (i = 0; i < count; i++) {
        s->ids[i] = ids[i];
        mask |= 1 << ids[i];
    }
    if (!func_0201b584(mask)) return FALSE;
    s->count = count;
    s->mask = mask;
    return TRUE;
}
