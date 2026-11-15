#include <nitro/types.h>

typedef struct UnkStruct_0235fbcc {
    u8 pad[4];
    s32 unk4;
} UnkStruct_0235fbcc;

u8 *func_020514e4(void);
u8 *func_02051474(void);

#pragma thumb on
u8 func_ov000_0235fbcc(UnkStruct_0235fbcc *s) {
    u8 *p;
    if (s->unk4 != 0)
        p = func_020514e4();
    else
        p = func_02051474();
    return *p;
}
#pragma thumb reset
