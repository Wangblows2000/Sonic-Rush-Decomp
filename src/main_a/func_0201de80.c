#include <nitro/types.h>

void func_0201deb4(void *a, u32 b, u32 c);
void func_02008ab4(void *a, void *b);

void func_0201de80(u8 *a, u32 b, u32 c, u32 d) {
    func_0201deb4(a, c, d);
    func_02008ab4(a, a + b);
}
