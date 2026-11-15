#include <nitro/types.h>

BOOL func_020156fc(u32 v);
void func_020168ec(u32 a, u32 b, void *c);
extern u8 *data_02080f1c;
void func_02016928(u32 a, u32 b, void *c) {
    if (func_020156fc(a)) *(u32 *)(data_02080f1c + (a - 1) * 4 + 0x14e8) = b;
    func_020168ec(a, b, c);
}
