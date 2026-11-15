#include <nitro/types.h>

typedef struct UnkStruct_0235fb50 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_0235fb50;

u8 *func_020514e4(void);
u8 *func_02051474(void);

#pragma thumb on
BOOL func_ov000_0235fb50(UnkStruct_0235fb50 *s) {
    u8 *p;
    if (s->unk4 != 0)
        p = func_020514e4();
    else
        p = func_02051474();
    if (p[3] == 0)
        return FALSE;
    return TRUE;
}
#pragma thumb reset
