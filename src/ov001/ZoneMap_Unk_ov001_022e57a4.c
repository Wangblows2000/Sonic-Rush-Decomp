#include <nitro/types.h>

typedef struct { s32 n; s32 a[24]; } UnkStruct_022e57a4;

#pragma thumb on
void ZoneMap_Unk_ov001_022e57a4(u8 *p, s32 i, s32 v) {
    UnkStruct_022e57a4 *e = &((UnkStruct_022e57a4 *)(*(u8 **)(p + 0xa4) + 0x784))[i];
    s32 j;
    for (j = 0; j < e->n; j++) {
        e->a[j] = v;
    }
}
#pragma thumb reset
