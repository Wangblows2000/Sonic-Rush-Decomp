#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022e4e64;

typedef struct {
    u8 pad0[0x9ac];
    UnkSprite_022e4e64 a[21];
    u8 padf94[0x1004 - 0xf94];
    UnkSprite_022e4e64 c[3];
    UnkSprite_022e4e64 b[8];
} UnkStruct_022e4e64;

void func_ov001_022e8470(UnkSprite_022e4e64 *s);

#pragma thumb on
void func_ov001_022e4e64(UnkStruct_022e4e64 *p) {
    s32 i;

    for (i = 0; i < 21; i++) {
        func_ov001_022e8470(&p->a[i]);
    }
    for (i = 0; i < 8; i++) {
        func_ov001_022e8470(&p->b[i]);
    }
    for (i = 0; i < 3; i++) {
        func_ov001_022e8470(&p->c[i]);
    }
}
#pragma thumb reset
