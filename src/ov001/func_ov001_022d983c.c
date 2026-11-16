#include <nitro/types.h>

typedef struct { s32 arr[7]; u8 pad[0x28 - 7 * 4]; } UnkStruct_022d983c_e;
typedef struct { u8 pad[0x24]; UnkStruct_022d983c_e e[1]; } UnkStruct_022d983c;
s32 Font_GetGlyphIndex(s32 a);

#pragma thumb on
void func_ov001_022d983c(UnkStruct_022d983c *p, s32 i) {
    s32 v = Font_GetGlyphIndex(0x20);
    s32 j;
    for (j = 0; j < 7; j++) {
        p->e[i].arr[j] = v;
    }
}
#pragma thumb reset
