#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    u32 flags;
} UnkStruct_022d57b4;

extern u8 data_02084d40[];
extern UnkStruct_022d57b4 data_02084db8;

void func_0200468c(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, BOOL draw, void *mtx);

#pragma thumb on
void func_ov001_022d57b4(void) {
    func_0200468c(0x5f000, -0x60000, -0x80000, 0x7f000, 0x1000, 0xc8000, 0x1000, FALSE, data_02084d40);
    data_02084db8.flags &= ~0x50;
}
#pragma thumb reset
