#include <nitro/types.h>

void func_02025268(u32 a, void *b, u32 c);

void func_02030918(u8 *s) {
    func_02025268(0x19, s + 0x24, 0xC);
    func_02025268(0x1C, s + 0x54, 3);
    func_02025268(0x1B, s + 0x18, 3);
}
