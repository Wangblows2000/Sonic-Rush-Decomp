#include <nitro/types.h>

void func_ov001_02302398(void *p);
BOOL func_ov001_02302408(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

void func_ov001_02301bac(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    func_ov001_02302398(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    if (func_ov001_02302408(p)) {
        return;
    }
    ExtraBossPlayer_ChangeState(p, 1);
    *(u32 *)(p + 0x14) &= ~0x700;
}
