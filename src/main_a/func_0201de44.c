#include <nitro/types.h>

void func_0201deb4(void *a, u32 b, u32 c);
void func_02008a88(void *a, void *b);
void func_02009e2c(void *a);

void func_0201de44(u8 *a, u32 b, u32 c, u32 d) {
    func_0201deb4(a, c, d);
    func_02008a88(a, a + b);
    func_02009e2c(a);
}
