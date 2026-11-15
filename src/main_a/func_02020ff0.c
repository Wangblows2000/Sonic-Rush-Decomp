#include <nitro/types.h>

void func_02021100(void *p);
void func_020210d0(void *p);

void func_02020ff0(void *p, u32 flags) {
    if (flags & 1) func_02021100(p);
    if (!(flags & 2)) return;
    func_020210d0(p);
}
