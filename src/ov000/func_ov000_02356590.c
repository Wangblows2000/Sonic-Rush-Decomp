#include <nitro/types.h>

typedef struct UnkStruct_02356590 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_02356590;

u8 *func_02051474(void);

#pragma thumb on
BOOL func_ov000_02356590(UnkStruct_02356590 *s) {
    if (s->unkC != 2)
        return FALSE;
    if (func_02051474()[2] != 0)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
