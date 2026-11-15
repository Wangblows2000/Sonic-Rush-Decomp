#include <nitro/types.h>

typedef struct { u32 flags; u8 pad[0xc]; s32 timer; } UnkStruct_02301dd4;
u8 *func_ov001_02300c94(void);
u8 *func_ov001_022ffce8(void);
void func_ov001_023024d0(void *p, s32 a);
void func_020366f4(s32 a, s32 b, s32 c);

void func_ov001_02301dd4(void) {
    u8 *p = func_ov001_02300c94();
    UnkStruct_02301dd4 *a = *(UnkStruct_02301dd4 **)(p + 0x1dc);
    func_ov001_023024d0(p, 5);
    *(u32 *)(*(u8 **)(p + 0x9c) + 8) |= 0x200;
    *(s32 *)(p + 0xa0) = 0;
    *(s32 *)(p + 0x1c8) = 0x3c;
    a->flags |= 2;
    a->timer = 0xb4;
    a->flags &= ~4;
    if (*(s32 *)(func_ov001_022ffce8() + 0x3c) == 0) {
        func_020366f4(0, 0, 0x21);
    } else {
        func_020366f4(0, 0, 0x2a);
    }
}
