#include <nitro/types.h>

extern u8 *data_022b4574;
void func_ov001_022c61c8(void *p);

void func_ov001_022c66a8(void) {
    func_ov001_022c61c8(*(void **)(data_022b4574 + 0x10));
}
