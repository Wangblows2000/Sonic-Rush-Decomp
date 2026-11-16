#include <nitro/types.h>

extern u8 *gTaskCurrent;
void func_ov001_022c61c8(void *p);

void func_ov001_022c66a8(void) {
    func_ov001_022c61c8(*(void **)(gTaskCurrent + 0x10));
}
