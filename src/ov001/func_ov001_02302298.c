#include <nitro/types.h>

void func_ov001_02302398(void *p);
BOOL func_ov001_02302408(void *p);
void func_ov001_02302140(void *p);
void func_ov001_023024d0(void *p, s32 a);

void func_ov001_02302298(u8 *p) {
    func_ov001_02302398(p);
    if (func_ov001_02302408(p)) {
        return;
    }
    func_ov001_02302140(p);
    if (*(s32 *)(p + 0x5c) != 0) {
        return;
    }
    if (*(s32 *)(p + 0x60) != 0) {
        return;
    }
    if (*(s32 *)(p + 0x50) != 0) {
        return;
    }
    if (*(s32 *)(p + 0x54) != 0) {
        return;
    }
    func_ov001_023024d0(p, 1);
    *(u32 *)(p + 0x14) &= ~0x700;
}
