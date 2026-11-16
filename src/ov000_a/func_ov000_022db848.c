#include <nitro/types.h>

typedef struct UnkStruct_022db848 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022db848;

extern UnkStruct_022db848 *gPlayers;

BOOL func_ov000_022db848(UnkStruct_022db848 *o) {
    s32 d = gPlayers->unk10 - o->unk10;
    BOOL f = d > 0;
    if ((d < 0 ? -d : d) <= 0x384ff)
        return f;
    return !f;
}
