#include <nitro/types.h>

typedef struct UnkStruct_022db9a8 {
    u8 pad[0xbbc];
    s32 unkBBC;
    u8 pad1[0xbc4 - 0xbc0];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
} UnkStruct_022db9a8;

BOOL Boss4_IsTiltPastLimits(UnkStruct_022db9a8 *s) {
    if (s->unkBBC >= s->unkBC8 && s->unkBC4 > 0)
        return TRUE;
    if (s->unkBBC <= s->unkBCC && s->unkBC4 < 0)
        return TRUE;
    return FALSE;
}
