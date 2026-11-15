#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 flags; } UnkStruct_022c4680;
extern UnkStruct_022c4680 data_022c4680;

#pragma thumb on
void func_ov001_022cb6d8(BOOL *p) {
    if (data_022c4680.flags & 1) {
        p[0] = TRUE;
    } else {
        p[0] = FALSE;
    }
    if (data_022c4680.flags & 2) {
        p[1] = FALSE;
    } else {
        p[1] = TRUE;
    }
}
#pragma thumb reset
