#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022e35d4;

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[4];
    UnkSprite_022e35d4 spr[4];
} UnkStruct_022e35d4;

void func_02033550(void *s, u16 id);
void func_0203344c(void *s, s32 a, s32 b);

#pragma thumb on
void func_ov001_022e35d4(UnkStruct_022e35d4 *p, s32 mode) {
    s32 i;

    if (p->mode != mode) {
        p->mode = mode;
        for (i = 0; i < 4; i++) {
            func_02033550(&p->spr[i], i);
            func_0203344c(&p->spr[i], 0, 0);
        }
    }
}
#pragma thumb reset
