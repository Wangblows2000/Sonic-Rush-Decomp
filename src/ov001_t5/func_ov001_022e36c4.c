#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022e36c4;

typedef struct {
    u8 pad0[0xc];
    UnkSprite_022e36c4 spr[6];
} UnkStruct_022e36c4;

void func_ov001_022e8470(UnkSprite_022e36c4 *s);

#pragma thumb on
void func_ov001_022e36c4(UnkStruct_022e36c4 *p) {
    s32 i;

    for (i = 0; i < 4; i++) {
        func_ov001_022e8470(&p->spr[i]);
    }
    func_ov001_022e8470(&p->spr[4]);
    func_ov001_022e8470(&p->spr[5]);
}
#pragma thumb reset
