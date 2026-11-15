#include <nitro/types.h>

void func_ov001_02302398(void *p);
void func_ov001_02302408(void *p);

void func_ov001_02302320(u8 *p) {
    func_ov001_02302398(p);
    *(s32 *)(p + 0x50) = -0x1000;
    func_ov001_02302408(p);
}
