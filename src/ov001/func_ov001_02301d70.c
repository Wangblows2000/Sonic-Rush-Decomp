#include <nitro/types.h>

void func_ov001_02302398(void *p);
BOOL func_ov001_02302408(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

void func_ov001_02301d70(u8 *p) {
    func_ov001_02302398(p);
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) > 0) {
        return;
    }
    *(s32 *)(p + 0x50) = 0;
    if (func_ov001_02302408(p)) {
        return;
    }
    ExtraBossPlayer_ChangeState(p, 1);
    *(u32 *)(p + 0x14) &= ~0x700;
}
