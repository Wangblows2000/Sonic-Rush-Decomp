#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa4];
    u8 *work;
} UnkStruct_022e3510;

void func_02033550(void *s, u16 anim);

#pragma thumb on
void func_ov001_022e3510(UnkStruct_022e3510 *p) {
    u8 *w = p->work + 0x1b08;

    func_02033550(w + 0x174, 4);
    *(u32 *)(w + 0x1a0) &= ~4;
}
#pragma thumb reset
