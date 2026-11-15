#include <nitro/types.h>

void func_0201deb4(void *a, u32 b, u32 c);
void func_02008a5c(void *a, void *b);
void func_02009de0(void *a);

void func_0201de08(u8 *a, u32 b, u32 c, u32 d) {
    func_0201deb4(a, c, d);
    func_02008a5c(a, a + b);
    func_02009de0(a);
}
