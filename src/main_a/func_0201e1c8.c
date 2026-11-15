#include <nitro/types.h>

u32 func_0201d0f8(u32 idx);
u32 func_0201d558(void);
u32 func_0201e244(u32 idx, void *func, u32 c, u32 d, u32 e);
void func_0201d0dc(u32 idx, u32 v);
void func_0201de80(void);

u32 func_0201e1c8(u32 idx, u32 b, BOOL c) {
    u32 r = func_0201d0f8(idx);
    if (r == 0) {
        r = func_0201e244(idx, func_0201de80, c ? func_0201d558() : 0, idx, b);
        if (c && r != 0) {
            func_0201d0dc(idx, r);
        }
    }
    return r;
}
