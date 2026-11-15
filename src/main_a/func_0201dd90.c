#include <nitro/types.h>

u8 *func_02009b60(u32 a, u32 b);
void func_02009ba8(u32 a, u32 b, u32 c);
void func_02008a5c(void *a, void *b);

void func_0201dd90(u8 *a, u32 b, u32 c, u32 d) {
    if (a == func_02009b60(c, d)) {
        func_02009ba8(c, d, 0);
    }
    func_02008a5c(a, a + b);
}
