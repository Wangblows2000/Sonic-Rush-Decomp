#include <nitro/types.h>

extern BOOL func_02008f48(void);
extern u32 func_02008f90(void);
extern s32 func_020090b8(void);
extern BOOL func_02009520(u32 a);
extern void func_0200927c(u32 a);
extern void func_02008fe8(void);

u32 func_02009484(u32 a)
{
    u32 r;
    if (!func_02008f48()) {
        return 0;
    }
    if ((r = func_02008f90()) != 0) {
        return r;
    }
    if (!(a & 1)) {
        return 0;
    }
    if (func_020090b8() > 0) {
        while (func_02009520(0) != 0) {
        }
        if ((r = func_02008f90()) != 0) {
            return r;
        }
    } else {
        func_0200927c(1);
    }
    func_02008fe8();
    do {
        func_02009520(1);
    } while ((r = func_02008f90()) == 0);
    return r;
}
