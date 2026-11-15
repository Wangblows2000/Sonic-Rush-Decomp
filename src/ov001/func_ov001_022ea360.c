#include <nitro/types.h>

typedef struct { u8 pad[8]; s32 prev; s32 cur; u8 pad2[0x18]; } UnkStruct_022ea360;

#pragma thumb on
void func_ov001_022ea360(UnkStruct_022ea360 *p, s32 i, s32 v) {
    p[i].prev = p[i].cur;
    p[i].cur = v;
}
#pragma thumb reset
